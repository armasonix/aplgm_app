/*
______ _      _____   _____________
| ___ \ |    / _ \ \ / /  ___| ___ \
| |_/ / |   / /_\ \ V /| |__ | |_/ /
|  __/| |   |  _  |\ / |  __||    /
| |   | |___| | | || | | |___| |\ \
\_|   \_____|_| |_/\_/ \____/\_| \_|

- Реализация класса логики игрока: движение, анимации, коллизии.
- Обработка визуального представления и взаимодействия с игровым миром.
- Управление состоянием персонажа: здоровье, бонусы, смерть.
*/

#include "Player.h"

Player::Player() 
{
    // Загружает текстуру
    if (!texture.loadFromFile(Constants::RESOURCES_PATH + "player.png"))
    {
        throw std::runtime_error("Failed to load player texture!");
    }
    sprite.setTexture(texture);

    // Настраивает размер и центрирование
    float scale = Constants::PLAYER_SIZE / texture.getSize().x * 1.2f;
    sprite.setScale(scale, scale);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    baseColor = sf::Color::Cyan;
    updateRotation();
    reset();
}

// Сброс персонажа
void Player::reset() 
{
    position = { Constants::SCREEN_WIDTH / 2.f, Constants::SCREEN_HEIGHT / 2.f };
    sprite.setPosition(position);
    speed = Constants::INIT_SPEED;
    direction = Direction::Right;
    updateRotation();
    sprite.setColor(sf::Color::Cyan);
    isBlinking = false;
}

// Логика движения персонажа и его ускорение
void Player::update(float deltaTime)
{
    switch (direction)
    {
    case Direction::Right: position.x += speed * deltaTime; break;
    case Direction::Up:    position.y -= speed * deltaTime; break;
    case Direction::Left:  position.x -= speed * deltaTime; break;
    case Direction::Down:  position.y += speed * deltaTime; break;
    }
    updateRotation();
    //speed += Constants::ACCELERATION * deltaTime;
    updateBlink();
}

// Анимация мигания персонажа при съедании яблока
void Player::updateBlink()
{
    if (isBlinking)
    {
        float elapsed = blinkClock.getElapsedTime().asSeconds();
        if (elapsed < Constants::BLINK_DURATION)
        {
            int phase = static_cast<int>(elapsed / 0.1f) % 2;
            sf::Color newColor = sprite.getColor();
            newColor.a =  phase ? 128 : 255;
            sprite.setColor(newColor);
        }
        else
        {
            sprite.setColor(sf::Color::Cyan);
            isBlinking = false;
        }
    }
}

// Обновляет поворот персонажа
void Player::updateRotation()
{
    switch (direction)
    {
        case Direction::Right:
            sprite.setRotation(0.f);
            break;
        case Direction::Up:
            sprite.setRotation(270.f);
            break;
        case Direction::Left:
            sprite.setRotation(180.f);
            break;
        case Direction::Down:
            sprite.setRotation(90.f);
            break;
    }
}

// Рендер персонажа
void Player::draw(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Player::setColor(const sf::Color& color) 
{
    baseColor = color;
    sprite.setColor(color);

    if (isBlinking)
    {
        isBlinking = false;
        sprite.setColor(baseColor);
    }
}

sf::Color Player::getColor() const 
{
    return sprite.getColor();
}

// Увеличивает скорость персонажа
void Player::increaseSpeed()
{
    speed += 15.f;
    if (speed > 300.f) speed = 300.f;
}

// Сбрасывает скорость персонажа
void Player::resetSpeed()
{
    speed = Constants::INIT_SPEED;
}

// Получает скорость персонажа
float Player::getSpeed() const
{
    return speed;
}