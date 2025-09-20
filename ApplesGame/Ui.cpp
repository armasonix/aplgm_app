/*
 _   _ _____
| | | |_   _|
| | | | | |
| | | | | |
| |_| |_| |_
 \___/ \___/

 - Реализация класса системы UI
 - Обработка и рендер игровых меню:
   * Главное меню
   * Меню паузы
   * Экран Game Over
 - Управление визуальными эффектами UI:
   * Мигание контуров текста
   * Анимации выделения пунктов
   * Переходы между состояниями
 - Интеграция с игровой логикой:
   * Обработка нажатий клавиш
   * Синхронизация с игровым состоянием
   * Отображение динамических данных счета
 - Работа с ресурсами:
   * Загрузка шрифтов
   * Управление звуками интерфейса
   * Настройка цветовых схем
 - Визуальная обратная связь:
   * Подсветка активных элементов
   * Анимации переходов
   * Эффекты взаимодействия
*/

#include <algorithm>
#include "UI.h"
#include "Enums.h"

UIHandler::UIHandler(const MenuConfig& config)
    : font(config.font), menuSound(config.menuSound), selectSound(config.selectSound),
           blinkSpeed(Constants::MENU_TITLE_OUTLINE_BLINK_SPEED)
{}

// Инициализация главного меню
void UIHandler::initMainMenu()
{
    mainMenu.title.setFont(font);
    mainMenu.title.setString("APPLES GAME");
    mainMenu.title.setCharacterSize(Constants::MENU_TITLE_SIZE);
    mainMenu.title.setFillColor(Constants::MENU_COLOR);
    mainMenu.title.setOutlineColor(sf::Color(208, 248, 20));
    mainMenu.title.setOutlineThickness(2.0f);
    mainMenu.title.setPosition(Constants::SCREEN_WIDTH / 2 - mainMenu.title.getLocalBounds().width / 2,
                               Constants::SCREEN_HEIGHT / 4);

    std::vector<std::string> mainItems = { "START GAME", "LEADERBOARD", "EXIT" };
    for (size_t i = 0; i < mainItems.size(); ++i)
    {
        sf::Text item;
        item.setFont(font);
        item.setString(mainItems[i]);
        item.setCharacterSize(Constants::MENU_ITEM_SIZE);
        item.setPosition(Constants::SCREEN_WIDTH / 2 - item.getLocalBounds().width / 2,
                         Constants::SCREEN_HEIGHT / 2 + i * 60);
        item.setFillColor(Constants::MENU_COLOR);
        mainMenu.items.push_back(item);
    }
    updateMenuVisuals(mainMenu);
}

// Инициализация меню паузы
void UIHandler::initPauseMenu()
{
    pauseMenu.title.setFont(font);
    pauseMenu.title.setString("GAME PAUSED");
    pauseMenu.title.setCharacterSize(50);
    pauseMenu.title.setPosition(Constants::SCREEN_WIDTH / 2 - 180, Constants::SCREEN_HEIGHT / 2 - 100);

    std::vector<std::string> pauseItems = { "CONTINUE", "RESTART", "MAIN MENU" };
    for (size_t i = 0; i < pauseItems.size(); ++i)
    {
        sf::Text item;
        item.setFont(font);
        item.setString(pauseItems[i]);
        item.setCharacterSize(Constants::MENU_ITEM_SIZE);
        item.setPosition(Constants::SCREEN_WIDTH / 2 - item.getLocalBounds().width / 2,
                         Constants::SCREEN_HEIGHT / 2 - 50 + i * 60);
        item.setFillColor(Constants::MENU_COLOR);
        pauseMenu.items.push_back(item);
    }
    updateMenuVisuals(pauseMenu);
}

// Действия главного меню
UIHandler::MenuAction UIHandler::handleMainMenuInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Up:
            updateMenuSelection(false, MenuState::MAIN_MENU);
            return MenuAction::NONE;

        case sf::Keyboard::Down:
            updateMenuSelection(true, MenuState::MAIN_MENU);
            return MenuAction::NONE;

        case sf::Keyboard::Enter:
            selectSound.play();
            switch (mainMenu.selectedIndex)
            {
            case 0: return MenuAction::START_GAME;
            case 1: return MenuAction::SHOW_LEADERBOARD;
            case 2: return MenuAction::EXIT;
            default: return MenuAction::NONE;
            }

        case sf::Keyboard::Escape:
            return MenuAction::EXIT;

        default:
            return MenuAction::NONE;
        }
    }
    return MenuAction::NONE;
}

// Действия меню паузы
UIHandler::MenuAction UIHandler::handlePauseMenuInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Up:
            updateMenuSelection(false, MenuState::PAUSE_MENU);
            return MenuAction::NONE;

        case sf::Keyboard::Down:
            updateMenuSelection(true, MenuState::PAUSE_MENU);
            return MenuAction::NONE;

        case sf::Keyboard::Enter:
            selectSound.play();
            switch (pauseMenu.selectedIndex)
            {
            case 0: return MenuAction::CONTINUE;
            case 1: return MenuAction::RESTART;
            case 2: return MenuAction::MAIN_MENU;
            default: return MenuAction::NONE;
            }

        case sf::Keyboard::Escape:
            return MenuAction::CONTINUE;

        default:
            return MenuAction::NONE;
        }
    }
    return MenuAction::NONE;
}

// Выбор режимы игры
int UIHandler::showModeSelectionMenu(sf::RenderWindow& window)
{
    std::vector<std::string> options = 
    {
        "[1] Limited Apples",
        "[2] Unlimited Apples",
        "[3] Acceleration Mode",
        "[4] No Accelerartion Mode",
        "[Enter] Start Playing"
    };

    int gameModeMask = 0;
    sf::Text optionText;
    optionText.setFont(font);
    optionText.setCharacterSize(32);
    optionText.setFillColor(sf::Color::White);
    optionText.setPosition(100, 100);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) 
            {
                switch (event.key.code) 
                {
                case sf::Keyboard::Num1:
                    gameModeMask ^= LIMITED_APPLES;
                    break;
                case sf::Keyboard::Num2:
                    gameModeMask ^= UNLIMITED_APPLES;
                    break;
                case sf::Keyboard::Num3:
                    gameModeMask ^= SPEED_UP;
                    break;
                case sf::Keyboard::Num4:
                    gameModeMask ^= NO_SPEED_UP;
                    break;
                case sf::Keyboard::Enter:
                    // Исключает конфликты
                    if ((gameModeMask & LIMITED_APPLES) && (gameModeMask & UNLIMITED_APPLES))
                        gameModeMask &= ~UNLIMITED_APPLES;
                    if ((gameModeMask & SPEED_UP) && (gameModeMask & NO_SPEED_UP))
                        gameModeMask &= ~NO_SPEED_UP;
                    return gameModeMask;
                default:
                    break;
                }
            }
        }

        window.clear();
        for (size_t i = 0; i < options.size(); ++i) 
        {
            std::string label = options[i];
            if (i < 4 && HasGameMode(gameModeMask, static_cast<GameMode>(1 << i))) 
            {
                label += " [ON]";
            }
            optionText.setString(label);
            sf::FloatRect bounds = optionText.getLocalBounds();
            optionText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            optionText.setPosition(Constants::SCREEN_WIDTH / 2.f, 200 + i * 50);

            window.draw(optionText);
        }
        window.display();
    }

    return gameModeMask;
}

// Обновляет выбор пунктов меню
void UIHandler::updateMenuSelection(bool moveDown, MenuState type)
{
    menuSound.play();
    Menu& menu = (type == MenuState::MAIN_MENU) ? mainMenu : pauseMenu;

    if (moveDown) 
    {
        menu.selectedIndex = (menu.selectedIndex + 1) % menu.items.size();
    }
    else {
        menu.selectedIndex = (menu.selectedIndex > 0) ? menu.selectedIndex - 1 : menu.items.size() - 1;
    }

    updateMenuVisuals(menu);
}

// Обновляет визуализацию меню
void UIHandler::updateMenuVisuals(Menu& menu)
{
    for (auto& item : menu.items)
    {
        item.setFillColor(Constants::MENU_COLOR);
        item.setStyle(sf::Text::Regular);
    }

    if (!menu.items.empty())
    {
        menu.items[menu.selectedIndex].setFillColor(sf::Color::White);
        menu.items[menu.selectedIndex].setStyle(sf::Text::Underlined);
    }
}

// Рендер главного меню
void UIHandler::drawMainMenu(sf::RenderWindow& window)
{
    float alpha = (sin(outlineBlinkClock.getElapsedTime().asSeconds() * outlineBlinkSpeed) + 1) * 127.5f;
    mainMenu.title.setOutlineColor(sf::Color(208, 248, 20, static_cast<sf::Uint8>(alpha)));

    window.draw(mainMenu.title);
    for (const auto& item : mainMenu.items)
    {
        window.draw(item);
    }
}

// Рендер меню паузы
void UIHandler::drawPauseMenu(sf::RenderWindow& window)
{
    window.draw(pauseMenu.title);
    for (const auto& item : pauseMenu.items)
    {
        window.draw(item);
    }
}

// Сброс меню паузы
void UIHandler::resetPauseMenu()
{
    pauseMenu.selectedIndex = 0;
    updateMenuVisuals(pauseMenu);
}

// Таблица рекордов
void UIHandler::drawLeaderboard(sf::RenderWindow& window,
    const std::vector<std::pair<std::string, int>>& rows,
    int highlightIndex,
    float startY,
    int maxRows)
{
    // Заголовок
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    title.setString("LEADERBOARD");

    // Выравнивание заголовка по центру
    {
        auto b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        const float titleY = (startY == 0.f)
            ? (Constants::SCREEN_HEIGHT / 2.f + 40.f)
            : startY;
        title.setPosition(Constants::SCREEN_WIDTH / 2.f, titleY);
    }
    window.draw(title);

    float y = title.getPosition().y + 40.f;

    const int shown = std::min<int>(maxRows, static_cast<int>(rows.size()));
    for (int i = 0; i < shown; ++i)
    {
        const std::pair<std::string, int>& row = rows[i];
        const std::string& name = row.first;
        int sc = row.second;

        sf::Text line;
        line.setFont(font);
        line.setCharacterSize(22);
        line.setString(std::to_string(i + 1) + ") " + name + " - " + std::to_string(sc));

        // Подсветка строки Player
        if (i == highlightIndex) 
        {
            line.setFillColor(sf::Color(255, 230, 80));
            line.setOutlineThickness(1.0f);
            line.setOutlineColor(sf::Color(60, 60, 0));
        }
        else 
        {
            line.setFillColor(sf::Color::White);
        }

        auto b = line.getLocalBounds();
        line.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        line.setPosition(Constants::SCREEN_WIDTH / 2.f, y);
        window.draw(line);

        y += 28.f;
    }
}

void UIHandler::drawLeaderboardScreen(
    sf::RenderWindow& window,
    const std::vector<std::pair<std::string, int>>& rows,
    int highlightIndex)
{
    // Затемняет фон
    sf::RectangleShape overlay(sf::Vector2f(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    // Заголовок
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::White);
    title.setString("HIGHLIGHTS");

    auto tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    const float titleY = Constants::SCREEN_HEIGHT * Constants::OVERLAY_TITLE_Y_RATIO;
    title.setPosition(Constants::SCREEN_WIDTH / 2.f, titleY);
    window.draw(title);

    // Таблица строк
    const float tableStartY =
        title.getPosition().y
        + tb.height * 0.5f
        + Constants::LEADERBOARD_GAP_FROM_TITLE;

    drawLeaderboard(window, rows, highlightIndex, tableStartY, /*maxRows=*/10);

    // Подсказка "назад"
    sf::Text hint;
    hint.setFont(font);
    hint.setCharacterSize(20);
    hint.setFillColor(sf::Color(200, 200, 200));
    hint.setString("Enter / Esc - back to Main Menu ");
    auto hb = hint.getLocalBounds();
    hint.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
    hint.setPosition(Constants::SCREEN_WIDTH / 2.f, tableStartY + 12 * 28.f);
    window.draw(hint);
}