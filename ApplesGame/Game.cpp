/*
 _____  ___ ___  ________
|  __ \/ _ \|  \/  |  ___|
| |  \/ /_\ \ .  . | |__
| | __|  _  | |\/| |  __|
| |_\ \ | | | |  | | |___
 \____|_| |_|_|  |_|____/

Основной класс игры. Отвечает за управление основным игровым циклом,
состояниями (меню, игра, победа, поражение), генерацией и обновлением 
игровых объектов такие как (игрок, яблоки, бонусы, враги, препятствия), 
обработкой столкновений и реализацией игровых режимов.

Ключевая логика:
- Обработка ввода игрока.
- Обновление состояния объектов (позиции, столкновения).
- Проверка условий победы/поражения.
- Поддержка различных режимов игры (через битовую маску GameModeMask).
- Визуализация экранов (меню, победа, поражение).
- Управление звуками и анимацией.
- Камера и экранные эффекты (тряска камеры и мигание).

Основные функции:
1. Инициализация:
   - Загрузка ресурсов (шрифты, звуки, музыка)
   - Настройка UI-элементов (меню, текст, оверлеи)
2. Игровой процесс:
   - Спавн объектов (яблоки, препятствия, враги)
   - Обработка коллизий (игрок/объекты, границы экрана)
   - Управление бонусной механикой (BonusApple)
3. Визуальные эффекты:
   - Тряска камеры (Camera Shake)
   - Анимации смерти и исчезновения
   - Плавные переходы между состояниями
4. Аудиосистема:
   - Фоновая музыка и звуковые эффекты
   - Управление аудиопотоками при смене состояний
5. UI-логика:
   - Отображение счета
   - Обработка меню (пауза, рестарт, выход)

Особенности реализации:
- Хранение объектов в векторах умных указателей
- Система проверки начальных коллизий при респавне
- Каскадные проверки столкновений через CollisionSystem
- Динамическая генерация позиций объектов с безопасной зоной
- Состояния игры реализованы через enum GameState
- Поддержка паузы с сохранением оригинальных цветов объектов
*/

#include <ctime>
#include <stdexcept>
#include <random>
#include <algorithm>
#include "Enemy.h"
#include "Game.h"
#include "CollisionSystem.h"

Game::Game() : window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "Apples Game"),
               uiHandler({ font, menuSound, menuSelectSound }),
               deathAnimationAlpha(255.0f), deathAnimationDuration(2.0f), isDeathAnimationActive(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    loadResources();
    state = MAIN_MENU;

    // Инициализация UI
    uiHandler.initMainMenu();
    uiHandler.initPauseMenu();

    window.setVisible(true);
    window.requestFocus();
    backgroundMusic.stop();
    menuMusic.play();

    fadeOverlay.setSize(sf::Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    gameObjectsFadeAlpha = 255.0f;
    isFadingObjects = false;
    initLeaderboardIfNeeded();

    appleGrid.init(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, Constants::GRID_CELL_SIZE);
}

void Game::handleMenuAction(UIHandler::MenuAction action) 
{
    switch (action) 
    {
    case UIHandler::MenuAction::START_GAME:
        if (!isTransitioning) 
        {
            menuSound.play();
            // Выбор режима игры
            gameModeMask = uiHandler.showModeSelectionMenu(window);
            isTransitioning = true;
            fadeAlpha = 0.0f;
        }
        break;
    case UIHandler::MenuAction::SHOW_LEADERBOARD:
        state = LEADERBOARD;
        break;
    case UIHandler::MenuAction::EXIT:
        window.close();
        break;
    default: break;
    }
}

void Game::handlePauseAction(UIHandler::MenuAction action) 
{
    switch (action) 
    {
    case UIHandler::MenuAction::CONTINUE:
        state = PLAYING;
        backgroundMusic.play();
        break;
    case UIHandler::MenuAction::RESTART:
        reset();
        state = PLAYING;
        break;
    case UIHandler::MenuAction::MAIN_MENU:
        state = MAIN_MENU;
        menuMusic.play();
        backgroundMusic.stop();
        break;
    default: break;
    }
}

// Камера шейк
void Game::activateCameraShake()
{
    shakeDuration = Constants::SHAKE_DURATION;
    shakeTimer = shakeDuration;
    shakeIntensity = Constants::SHAKE_INTENSITY;
}

void Game::loadResources()
{
    if (!font.loadFromFile(Constants::RESOURCES_PATH + "Roboto-Regular.ttf"))
        throw std::runtime_error("Failed to load font.");
    // Загружает звуки
    auto loadSound = [](sf::SoundBuffer& buffer, const std::string& path)
        {
            if (!buffer.loadFromFile(Constants::RESOURCES_PATH + path))
                throw std::runtime_error("Failed to load sound: " + path);
        };

    loadSound(appleSoundBuffer, "apple.wav");
    loadSound(bonusSoundBuffer, "bonus.wav");
    loadSound(gameOverSoundBuffer, "game_over.wav");
    loadSound(menuSoundBuffer, "menu.wav");
    loadSound(menuSoundSelectBuffer, "menu_select.wav");
    loadSound(winSoundBuffer, "win.wav");

    // Инициализирует звуки
    appleSound.setBuffer(appleSoundBuffer);
    bonusSound.setBuffer(bonusSoundBuffer);
    gameOverSound.setBuffer(gameOverSoundBuffer);
    menuSound.setBuffer(menuSoundBuffer);
    menuSelectSound.setBuffer(menuSoundSelectBuffer);
    winSound.setBuffer(winSoundBuffer);

    // Загружает музыку
    if (!backgroundMusic.openFromFile(Constants::RESOURCES_PATH + Constants::BACKGROUND_MUSIC))
        throw std::runtime_error("Failed to load background music.");
    backgroundMusic.setVolume(Constants::BACKGROUND_MUSIC_VOLUME);
    backgroundMusic.setLoop(true);

    if (!menuMusic.openFromFile(Constants::RESOURCES_PATH + Constants::MENU_MUSIC))
        throw std::runtime_error("Failed to load main menu music!");
    menuMusic.setLoop(true);

    // Инициализирует текст
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(Constants::SCREEN_WIDTH - 150, 10);

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setOutlineColor(sf::Color::Red);
    gameOverText.setOutlineThickness(2.0f);
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(sf::Color::White);

    // Центральное расположение текста Game Over
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);
    gameOverText.setPosition(Constants::SCREEN_WIDTH / 2, Constants::SCREEN_HEIGHT / 2);

    // Инициализирует оверлэй
    gameOverOverlay.setSize(sf::Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));
    gameOverOverlay.setFillColor(sf::Color(0, 0, 0, 0));

}


// Сброс
void Game::reset()
{
    justStarted = true;
    player.reset();
    player.resetSpeed();
    spawnObstacles();
    spawnApples();
    bonusApple.reset();
    score = 0;
    lastBonusScore = 0;
    gameOverSoundPlayed = false;
    gameOverClock.restart();
    state = PLAYING;

    // Установка позиции персонажа после спавна объектов
    player.position = 
    {
        Constants::SCREEN_WIDTH / 2.0f,
        Constants::SCREEN_HEIGHT / 2.0f
    };
    //  Дополнительная проверка начальной позиции
    bool initialCollision = false;
    for (const auto& obs : obstacles) 
    {
        if (Collision::circleRectCollision(
            player,
            Constants::PLAYER_SIZE / 2,
            *obs,
            obs->getSize())) 
        {
            initialCollision = true;
            break;
        }
    }

    if (initialCollision) 
    {
        // Респавн при коллизии
        spawnObstacles();
    }
    backgroundMusic.stop();
    menuMusic.stop();

    if (state == PLAYING) 
    {
        backgroundMusic.setVolume(Constants::BACKGROUND_MUSIC_VOLUME);
        backgroundMusic.play();
    }
    else if (state == MAIN_MENU)
    {
        menuMusic.play();
    }

    uiHandler.resetPauseMenu();

    // Анимация мигания экрана
    isBlinking = false;
    blinkTimer = 0.0f;

    // Камера шейк
    cameraShakeOffset = { 0, 0 };
    shakeTimer = 0.0f;
    shakeIntensity = 0.0f;

    // Fade анимация всех игровых объектов при GAME OVER
    gameObjectsFadeAlpha = 255.0f;
    isFadingObjects = false;

    // Спавн противника
    spawnEnemies();

    // Анимация смерти персонажа
    deathAnimationAlpha = 255.0f;
    isDeathAnimationActive = false;

    // Анимация Game Over текста
    gameOverBlinkClock.restart();

    // Анимация текста очков
    scoreColorClock.restart();
}

// Спавнит яблоки
void Game::spawnApples()
{
    apples.clear();

    int numApples = Constants::NUM_APPLES;

    if (HasGameMode(gameModeMask, GameMode::LIMITED_APPLES))
    {
        numApples = rand() % 6 + 5; // случайно количество яблок
    }
    else if (HasGameMode(gameModeMask, GameMode::UNLIMITED_APPLES))
    {
        numApples = Constants::NUM_APPLES;
    }

    for (int i = 0; i < numApples; ++i)
    {
        auto apple = std::make_unique<Apple>();
        do
        {
            apple->position = randomPosition();
        } 
        while (checkCollision(*apple));
        apples.push_back(std::move(apple));
    }
    appleGrid.rebuild(apples);
    appleCandidates.reserve(apples.size() / 3);
}

// Спавнит препятствия
void Game::spawnObstacles() 
{
    obstacles.clear();

    for (int i = 0; i < Constants::NUM_OBSTACLES; ++i) 
    {
        bool collisionWithPlayer;
        std::unique_ptr<Obstacle> obstacle;

        do 
        {
            collisionWithPlayer = false;

            // Генерация размеров препятствия
            float width = Constants::MIN_OBSTACLE_SIZE +
                static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Constants::MAX_OBSTACLE_SIZE - Constants::MIN_OBSTACLE_SIZE)));

            float height = Constants::MIN_OBSTACLE_SIZE +
                static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Constants::MAX_OBSTACLE_SIZE - Constants::MIN_OBSTACLE_SIZE)));

            // Создание препятствия
            obstacle = std::make_unique<Obstacle>(width, height);
            obstacle->position = randomPosition();

            // Явная проверка коллизии с игроком
            sf::FloatRect playerBounds = player.getBounds();
            sf::FloatRect obstacleBounds = obstacle->getBounds();

            if (playerBounds.intersects(obstacleBounds)) 
            {
                collisionWithPlayer = true;
                continue;
            }

        } 
        while (checkCollision(*obstacle) || collisionWithPlayer);

        obstacles.push_back(std::move(obstacle));
    }
}

// Спавнит противников
void Game::spawnEnemies()
{
    enemies.clear();
    for (int i = 0; i < Constants::NUM_ENEMIES; ++i)
    {
        auto enemy = std::make_unique<Enemy>();
        do 
        {
            enemy->position = randomPosition();
        } 
        while (checkCollision(*enemy));
        enemies.push_back(std::move(enemy));
    }
}

// Случайная позиция на экране
sf::Vector2f Game::randomPosition() const
{
    // Генерирует позиции дальше от центра
    const float safeZone = 60.f; // Минимальное расстояние от персонажа

    return
    {
        // Генерация в пределах экрана, исключая центральную зону
        20.0f + safeZone + static_cast<float>(rand() % (Constants::SCREEN_WIDTH - 40 - static_cast<int>(safeZone * 2))),
        20.0f + safeZone + static_cast<float>(rand() % (Constants::SCREEN_HEIGHT - 40 - static_cast<int>(safeZone * 2)))
    };
}

// Проверяет коллизии
bool Game::checkCollision(const GameObject& obj) const
{
    // Проверяет коллизию с персонажем
    if (&obj != &player && Collision::circleCollide(obj, player, Constants::APPLE_SIZE / 2,
        Constants::PLAYER_SIZE / 2))
        return true;

    // Проверяет коллизию с яблоками
    for (const auto& apple : apples)
        if (apple.get() != &obj && apple->active && Collision::circleCollide(obj, *apple, Constants::APPLE_SIZE / 2,
            Constants::APPLE_SIZE / 2))
            return true;

    // Проверяет коллизию с препятствиями
    for (const auto& obstacle : obstacles)
        if (obstacle.get() != &obj && Collision::circleRectCollision(obj, Constants::APPLE_SIZE / 2,
            *obstacle, obstacle->getSize()))
            return true;

    return false;
}

// Проверяет коллизию с границами экрана
void Game::checkBoundaries()
{
    // Использует точные границы с учетом центра
    const float halfSize = Constants::PLAYER_SIZE / 2.0f;
    const float left = player.position.x - halfSize;
    const float right = player.position.x + halfSize;
    const float top = player.position.y - halfSize;
    const float bottom = player.position.y + halfSize;

    // Проверяет с запасом в 1 пиксель
    if (left < 1.0f || right > Constants::SCREEN_WIDTH - 1.0f || top < 1.0f || bottom > Constants::SCREEN_HEIGHT - 1.0f)
    {
        triggerGameOver(CollisionType::Boundary);
        activateCameraShake();
    }
}

// Проверяет коллизию с препятствиями
void Game::checkObstaclesCollision()
{
    for (const auto& obstacle : obstacles)
    {
        if (Collision::circleRectCollision(player, Constants::PLAYER_SIZE / 2, *obstacle, obstacle->getSize()))
        {
            triggerGameOver(CollisionType::Obstacle);
            activateCameraShake();
        }
    }
}

// Проверяет коллизию с яблоками
void Game::checkAppleCollision()
{
    // Сбор кандидатов из 3×3 ячеек вокруг игрока
    appleGrid.collectNear(player.position, appleCandidates);

    for (int idx : appleCandidates)
    {
        if (idx < 0 || idx >= static_cast<int>(apples.size())) continue;
        auto& apple = apples[idx];
        if (!apple->active) continue;

        if (Collision::circleCollide(player, *apple,
            Constants::PLAYER_SIZE / 2,
            Constants::APPLE_SIZE / 2))
        {
            score++;
            appleSound.play();
            if (HasGameMode(gameModeMask, GameMode::SPEED_UP))
                player.increaseSpeed();

            player.isBlinking = true;
            player.blinkClock.restart();

            // обновляет сетки только точечно
            if (HasGameMode(gameModeMask, GameMode::UNLIMITED_APPLES))
            {
                // респавнит в новой позиции
                const sf::Vector2f oldPos = apple->position;

                do 
                {
                    apple->position = randomPosition();
                } 
                while (checkCollision(*apple));

                // перемещает индекс apple в сетке без rebuild
                appleGrid.move(idx, oldPos, apple->position);
            }
            else
            {
                // LIMITED: деактивирует яблоко и удаляет его индекс из сетки
                appleGrid.erase(idx, apple->position);
                apple->active = false;
            }
        }
    }
}

// Обрабатывает инпут с клавиатуры
void Game::handlePlayerInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player.direction = Direction::Right;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        player.direction = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.direction = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        player.direction = Direction::Down;
}

// Взаимодействие с бонусным яблоком
void Game::updateBonusApple()
{
    if (!HasGameMode(gameModeMask, GameMode::SPEED_UP))
        return;

    if (score - lastBonusScore >= Constants::BONUS_SCORE_INTERVAL && !bonusApple)
    {
        bonusApple = std::make_unique<BonusApple>();
        do
        {
            bonusApple->position = randomPosition();
        } 
        while (checkCollision(*bonusApple));
        lastBonusScore = score;
    }

    if (bonusApple)
    {
        bonusApple->update();
        if (bonusApple->isExpired())
        {
            bonusApple.reset();
        }
        else if (Collision::circleCollide(player, *bonusApple, Constants::PLAYER_SIZE / 2, Constants::APPLE_SIZE / 2))
        {
            score += Constants::BONUS_SCORE_VALUE;
            player.speed *= Constants::SPEED_REDUCTION_FACTOR;
            bonusSound.play();
            bonusApple.reset();
        }
    }
}

// Триггер Win
void Game::triggerWin()
{
    backgroundMusic.stop();
    winSoundPlayed = false;
    state = WIN;
    winTimer.restart(); // Сбрасывает таймер экрана победы
}

// Триггер Game Over
void Game::triggerGameOver(CollisionType type)
{
    if (state != GAME_OVER && !isBlinking)
    {
        state = GAME_OVER;
        gameOverFadeAlpha = 0.0f;
        isFadingObjects = true;
        gameObjectsFadeAlpha = 255.0f;
        gameOverClock.restart();
        if (!gameOverSoundPlayed)
        {
            isBlinking = true;
            blinkTimer = 0.0f;
            blinkColor = sf::Color(255, 50, 50, 150);
            backgroundMusic.stop();
            menuMusic.play();
            gameOverSound.play();
            gameOverSoundPlayed = true;
        }
        switch (type)
        {
            case CollisionType::Obstacle:
                blinkColor = Constants::OBSTACLE_BLINK_COLOR;
                break;
            case CollisionType::Boundary:
                blinkColor = Constants::BOUNDARY_BLINK_COLOR;
                break;
            case CollisionType::Enemy:
                blinkColor = Constants::ENEMY_BLINK_COLOR;
                break;
        }
        blinkColor.a = 0;
        activateCameraShake();
        isDeathAnimationActive = true;
        deathAnimationClock.restart();
    }
}

// Экран Game Over
void Game::drawGameOverScreen()
{
    // Мигание контура через синус
    float alpha = (sin(gameOverBlinkClock.getElapsedTime().asSeconds() * Constants::GAME_OVER_BLINK_SPEED) + 1) * 127.5f;
    gameOverText.setOutlineColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(alpha)));

    gameOverText.setString("GAME OVER!\nFinal Score: " + std::to_string(score));
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.width/2, textBounds.height/2);
    const float titleY = Constants::SCREEN_HEIGHT * Constants::OVERLAY_TITLE_Y_RATIO;
    gameOverText.setPosition(Constants::SCREEN_WIDTH / 2, titleY);
    window.draw(gameOverText);
}

// Экран Win
void Game::drawWinScreen()
{
    // Мигание контура через синус
    float alpha = (sin(gameOverBlinkClock.getElapsedTime().asSeconds() * Constants::GAME_OVER_BLINK_SPEED) + 1) * 127.5f;
    gameOverText.setOutlineColor(sf::Color(0, 255, 0, static_cast<sf::Uint8>(alpha))); 

    gameOverText.setString("YOU WIN!\nFinal Score: " + std::to_string(score));
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    const float titleY = Constants::SCREEN_HEIGHT * Constants::OVERLAY_TITLE_Y_RATIO;
    gameOverText.setPosition(Constants::SCREEN_WIDTH / 2, titleY);
    window.draw(gameOverText);
}

// Главный игровой цикл
void Game::run()
{
    menuMusic.play();
    sf::Clock frameClock;
    while (window.isOpen())
    {
        sf::Time deltaTime = frameClock.restart();
        handleEvents();
        update(deltaTime.asSeconds());
        render();
    }
}

// Обрабочик игровых эвентов
void Game::handleEvents() 
{
    sf::Event event;
    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) window.close();

        if (state == LEADERBOARD)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter ||
                    event.key.code == sf::Keyboard::Escape)
                {
                    state = MAIN_MENU;
                    uiHandler.initMainMenu();
                }
            }
            continue;
        }

        if (state == MAIN_MENU) 
        {
            auto action = uiHandler.handleMainMenuInput(event);
            handleMenuAction(action);
        }
        else if (state == PAUSED) 
        {
            auto action = uiHandler.handlePauseMenuInput(event);
            handlePauseAction(action);
        }
        else 
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) 
            {
                if (state != GAME_OVER) 
                {
                    bool wasPaused = (state == PAUSED);
                    state = (state == PAUSED) ? PLAYING : PAUSED;

                    // Приостанавливает / возобновляем таймеры противников
                    for (auto& enemy : enemies)
                    {
                        if (wasPaused) enemy->resumeTimers();
                        else enemy->pauseTimers();
                    }

                    menuSound.play();
                    if (state == PAUSED) 
                    {
                        uiHandler.resetPauseMenu();
                        backgroundMusic.pause();
                    }
                    else 
                    {
                        backgroundMusic.setVolume(Constants::BACKGROUND_MUSIC_VOLUME);
                        backgroundMusic.play();
                    }
                }
            }
        }
    }
}

void Game::initLeaderboardIfNeeded()
{
    if (leaderboardInitialized) return;

    // Имена ботов
    static const std::vector<std::string> kNames = 
    {
        "Artem", "Niko", "Ghost", "Neo", "Viper", "Neko", "Frost", "Sigma",
        "Super"
    };

    std::mt19937 rng(static_cast<uint32_t>(std::random_device{}()));
    std::uniform_int_distribution<int> distCount(7, 11);   // количество ботов
    std::uniform_int_distribution<int> distScore(50, 300); // очки ботов

    int botCount = distCount(rng);

    // исключает повтор имен
    std::vector<std::string> pool = kNames;
    std::shuffle(pool.begin(), pool.end(), rng);
    pool.resize(std::min<int>(botCount, (int)pool.size()));

    leaderboard.clear();
    leaderboard.reserve(botCount + 1);

    for (const auto& nm : pool) 
    {
        leaderboard.push_back({ nm, distScore(rng), false });
    }

    // Добавляет строку игрока начинается с 0 очков
    leaderboard.push_back({ "Player", 0, true });

    // Сортировка по убыванию очков
    std::stable_sort(leaderboard.begin(), leaderboard.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) 
        {
            return a.score > b.score;
        });

    leaderboardInitialized = true;
}

void Game::setPlayerScoreToLeaderboard(int value)
{
    // Гарантированно один Player в таблице
    auto it = std::find_if(leaderboard.begin(), leaderboard.end(),
        [](const ScoreEntry& e) { return e.isPlayer; });

    if (it == leaderboard.end()) 
    {
        leaderboard.push_back({ "Player", value, true });
    }
    else 
    {
        it->score = value;
    }

    // Пересортировать по убыванию
    std::stable_sort(leaderboard.begin(), leaderboard.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) 
        {
            return a.score > b.score;
        });
}

void Game::buildLeaderboardRows(std::vector<std::pair<std::string, int>>& outRows, int& outPlayerIndex) const
{
    outRows.clear();
    outRows.reserve(leaderboard.size());
    outPlayerIndex = -1;

    for (size_t i = 0; i < leaderboard.size(); ++i) 
    {
        outRows.emplace_back(leaderboard[i].name, leaderboard[i].score);
        if (leaderboard[i].isPlayer) outPlayerIndex = static_cast<int>(i);
    }
}

// Обновление
void Game::update(float deltaTime)
{
    // Обновляет режим игры с ограниченными яблоками
    if (HasGameMode(gameModeMask, GameMode::LIMITED_APPLES))
    {
        bool allCollected = std::all_of(apples.begin(), apples.end(), [](const std::unique_ptr<Apple>& a)
            {
                return !a->active;
            });

        if (allCollected && state == PLAYING)
        {
            triggerWin();
            return;
        }
    }

    // Обновляет камера шейк
    if (shakeTimer > 0.0f)
    {
        shakeTimer -= deltaTime;

        // Вычисляет интенсивность камера шейка
        float progress = shakeTimer / shakeDuration;
        float currentIntensity = shakeIntensity * progress;

        // Генерация случайного оффсета камеры
        cameraShakeOffset.x = (rand() % 100 - 50) * 0.01f * currentIntensity;
        cameraShakeOffset.y = (rand() % 100 - 50) * 0.01f * currentIntensity;

        // Сброс эффекта по таймауту
        if (shakeTimer <= 0.0f)
        {
            cameraShakeOffset = { 0, 0 };
            shakeTimer = 0.0f;
        }
    }

    if (state == WIN)
    {
        if (winTimer.getElapsedTime().asSeconds() >= 4.0f)
        {
            state = MAIN_MENU;
            menuMusic.play();
            uiHandler.resetPauseMenu();
        }
        return;
    }

    if (state == GAME_OVER && gameOverFadeAlpha < 255.0f)
    {
        gameOverFadeAlpha += Constants::GAME_OVER_FADE_SPEED * deltaTime;
        {
            if (gameOverFadeAlpha > 255.0f) gameOverFadeAlpha = 255.0f;
            gameOverOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(gameOverFadeAlpha)));
        }

        if (isFadingObjects)
        {
            // Уменьшает прозрачность объектов
            gameObjectsFadeAlpha -= Constants::GAME_OBJECT_FADE_SPEED * deltaTime;
            gameObjectsFadeAlpha = std::max(gameObjectsFadeAlpha, 0.0f);

            if (gameObjectsFadeAlpha <= 0.0f)
            {
                isFadingObjects = false;
                gameOverFadeAlpha = 0.0f; // Показывает оверлей
            }
        }
        else
        {
            // Затемнение экрана после исчезновения объектов
            gameOverFadeAlpha += Constants::GAME_OVER_FADE_SPEED * deltaTime;
            gameOverFadeAlpha = std::min(gameOverFadeAlpha, 255.0f);
        }

        gameOverOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(gameOverFadeAlpha)));
    }

    if (isBlinking)
    {
        blinkTimer += deltaTime;

        // Анимация мигания
        float alpha = (sin(blinkTimer * Constants::BLINK_FREQUENCY) * 0.5f * 0.5f) * Constants::MAX_BLINK_ALPHA;
        blinkColor.a = static_cast<sf::Uint8>(alpha);

        // Завершение анимации
        if (blinkTimer >= Constants::BLINK_DURATION_SCREEN)
        {
            isBlinking = false;
            state = GAME_OVER;
            backgroundMusic.stop();
            menuMusic.play();
        }
    }

    if (isTransitioning)
    {
        // Увеличивает прозрачность
        fadeAlpha += Constants::FADE_SPEED * deltaTime;

        if (fadeAlpha >= 255.0f)
        {
            // Завершает Fade анимацию
            isTransitioning = false;
            menuMusic.stop();
            reset();
            //state = PLAYING;
            backgroundMusic.setVolume(Constants::BACKGROUND_MUSIC_VOLUME);
            backgroundMusic.play();
        }
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }

    // Плавное исчезновение затемнения после перехода
    if (state == PLAYING && fadeAlpha > 0.0f)
    {
        fadeAlpha -= Constants::FADE_SPEED * deltaTime * 2;
        if (fadeAlpha < 0.0f) fadeAlpha = 0.0f;
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }

    if (state == PLAYING)
    {
        if (justStarted) 
        {
            justStarted = false;
            return; // пропускает первую проверку столкновений и апдейтов
        }

        handlePlayerInput();
        player.update(deltaTime);
        checkBoundaries();
        checkObstaclesCollision();
        checkAppleCollision();
        updateBonusApple();

        // Обновляет противников только в режиме PLAYING
        for (auto& enemy : enemies) 
        {
            enemy->update(deltaTime, obstacles);

            if (Collision::circleCollide(player, *enemy,
                Constants::PLAYER_SIZE / 2, Constants::PLAYER_SIZE / 2))
            {
                triggerGameOver(CollisionType::Enemy);
                activateCameraShake();
            }
        }

    }
    else if (state == GAME_OVER)
    {
        if (gameOverClock.getElapsedTime().asSeconds() > 4.0f)
        {
            reset();
            state = PLAYING;
        }
    justStarted = false;
    }

    // Затемнение экрана
    window.draw(gameOverOverlay);

    // Плавный рендер текста
    float textAlpha = std::min(gameOverFadeAlpha * 2.0f, 255.0f);
    gameOverText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(textAlpha)));

    // Рендер текста с эффектом шейка
    const float offset = 2.0f * (gameOverFadeAlpha / 255.0f);
    gameOverText.setPosition(Constants::SCREEN_WIDTH / 2 + offset, Constants::SCREEN_HEIGHT / 2);
    window.draw(gameOverText);
    gameOverText.setPosition(Constants::SCREEN_WIDTH / 2 - offset, Constants::SCREEN_HEIGHT / 2);
    window.draw(gameOverText);

    if (state == GAME_OVER && isFadingObjects)
    {
        gameObjectsFadeAlpha -= Constants::GAME_OBJECT_FADE_SPEED * deltaTime;
        if (gameObjectsFadeAlpha < 0.0f) 
        {
            gameObjectsFadeAlpha = 0.0f;
            isFadingObjects = false;
        }
    }

    if (state == GAME_OVER && isDeathAnimationActive) 
    {
        float elapsedTime = deathAnimationClock.getElapsedTime().asSeconds();
        float progress = elapsedTime / deathAnimationDuration;

        // Плавное уменьшение прозрачности
        deathAnimationAlpha = 255.0f * (1.0f - progress);

        // Изменение цвета на красный
        player.sprite.setColor(sf::Color(255, 0, 0, static_cast<sf::Uint8>(deathAnimationAlpha)));

        // Завершение анимации
        if (progress >= 1.0f) 
        {
            isDeathAnimationActive = false;
            deathAnimationAlpha = 0.0f;
        }
    }

    // Анимация цвета счета с перебором rgb
    float time = scoreColorClock.getElapsedTime().asSeconds();

    // RGB смена цвета
    sf::Uint8 r = static_cast<sf::Uint8>(sin(time * 2.5f) * 127 + 128);
    sf::Uint8 g = static_cast<sf::Uint8>(sin(time * 2.2f) * 127 + 128);
    sf::Uint8 b = static_cast<sf::Uint8>(sin(time * 1.9f) * 127 + 128);

    scoreText.setFillColor(sf::Color(r, g, b));
}

// Ренедер всех объектов и UI
void Game::render() 
{
    window.clear();

    sf::View originalView = window.getView();

    // Применение камера шейка
    if (shakeTimer > 0.0f) 
    {
        sf::View shakeView = originalView;
        shakeView.move(cameraShakeOffset);
        window.setView(shakeView);
    }

    if (state == MAIN_MENU) 
    {
        uiHandler.drawMainMenu(window);
        if (isTransitioning) window.draw(fadeOverlay);
        window.display();
        return;
    }
    else if (state == LEADERBOARD)
    {
        std::vector<std::pair<std::string, int>> rows;
        int playerIndex = -1;
        buildLeaderboardRows(rows, playerIndex);

        uiHandler.drawLeaderboardScreen(window, rows, playerIndex);
        window.display();
        return;
    }

    // Рендер игровых объектов
    if (state != MAIN_MENU) 
    {
        std::vector<sf::Color> originalColors;

        // При паузе сохраняет оригинальный цвет и устанавливает градации серого
        if (state == PAUSED) 
        {
            originalColors.push_back(player.sprite.getColor());
            player.sprite.setColor(sf::Color(Constants::GRAY_COLOR_4));

            // Устанавливаем серый цвет для яблок
            for (auto& apple : apples) 
            {
                originalColors.push_back(apple->getColor());
                apple->setColor(sf::Color(Constants::GRAY_COLOR));
            }

            // Устанавливаем серый цвет для препятствий
            for (auto& obstacle : obstacles) 
            {
                originalColors.push_back(obstacle->getColor());
                obstacle->setColor(sf::Color(Constants::GRAY_COLOR_3));
            }

            // Устанавливаем серый цвет для противников
            for (auto& enemy : enemies) 
            {
                originalColors.push_back(enemy->getColor());
                enemy->setColor(sf::Color(Constants::GRAY_COLOR_2));
            }
        }

        for (const auto& apple : apples) apple->draw(window);
        for (const auto& obstacle : obstacles) obstacle->draw(window);
        player.draw(window);
        if (bonusApple) bonusApple->draw(window);
        for (const auto& enemy : enemies) enemy->draw(window);

        // Восстанавливаем оригинальный цвет
        if (state == PAUSED) 
        {
            size_t index = 0;
            player.sprite.setColor(originalColors[index++]);
            for (auto& apple : apples) apple->setColor(originalColors[index++]);
            for (auto& obstacle : obstacles) obstacle->setColor(originalColors[index++]);
            for (auto& enemy : enemies) enemy->setColor(originalColors[index++]);
        }

        if (state == GAME_OVER) 
        {
            player.draw(window);
        }
        else 
        {
            player.draw(window);
        }
    }

    window.setView(originalView);

    // Рендер очков
    scoreText.setString("Score: " + std::to_string(score));
    window.draw(scoreText);

    if (state == PAUSED) 
    {
        uiHandler.drawPauseMenu(window);
    }
    else if (state == GAME_OVER)
    {
        window.draw(gameOverOverlay);
        drawGameOverScreen();

        // Обновляет очки игрока и пересортировывает таблицу
        setPlayerScoreToLeaderboard(score);

        std::vector<std::pair<std::string, int>> rows;
        rows.reserve(leaderboard.size());
        int playerIndex = -1;
        for (size_t i = 0; i < leaderboard.size(); ++i) 
        {
            rows.emplace_back(leaderboard[i].name, leaderboard[i].score);
            if (leaderboard[i].isPlayer) playerIndex = static_cast<int>(i);
        }

        // Рендер таблицы под заголовком Game Over
        const float tableStartY =
            gameOverText.getPosition().y
            + gameOverText.getLocalBounds().height * 0.5f
            + Constants::LEADERBOARD_GAP_FROM_TITLE;

        uiHandler.drawLeaderboard(window, rows, playerIndex, tableStartY, 10);
    }

    // Рендер экрана победы
    else if (state == WIN)
    {
        if (!winSoundPlayed) 
        {
            winSound.play();
            winSoundPlayed = true;
        }

        window.draw(gameOverOverlay);
        drawWinScreen();

        // Обновляет очки игрока и пересортировывает таблицу
        setPlayerScoreToLeaderboard(score);

        std::vector<std::pair<std::string, int>> rows;
        rows.reserve(leaderboard.size());
        int playerIndex = -1;
        for (size_t i = 0; i < leaderboard.size(); ++i) 
        {
            rows.emplace_back(leaderboard[i].name, leaderboard[i].score);
            if (leaderboard[i].isPlayer) playerIndex = static_cast<int>(i);
        }

        // Рендер таблицы под заголовком Win
        const float tableStartY =
            gameOverText.getPosition().y
            + gameOverText.getLocalBounds().height * 0.5f
            + Constants::LEADERBOARD_GAP_FROM_TITLE;

        uiHandler.drawLeaderboard(window, rows, playerIndex, tableStartY, 10);
    }

    if (isTransitioning) window.draw(fadeOverlay);

    window.display();
}