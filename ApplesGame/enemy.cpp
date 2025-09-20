/*
 _____ _   _ ________  ____   __
|  ___| \ | |  ___|  \/  \ \ / /
| |__ |  \| | |__ | .  . |\ V /
|  __|| . ` |  __|| |\/| | \ /
| |___| |\  | |___| |  | | | |
\____/\_| \_|____/\_|  |_/ \_/

 - ���������� ��������� ������� ������
 - �������� ��������:
   * ��������� �������������� � ���������� �����������
   * ����� �����������
   * ����������� �������� � �������� ������
   * ���������� ������������� (��������)

 - ����������� ��:
   * ������������ ������ ����� ����������� (1.0-3.0 ���)
   * ������������� ������� � ������ ������
   * ������� ������� �������� � ����������

 - ������� ���������� ����������:
   * �����/������������� ���������� ��������
   * ��������� ����� ��� ���������� �������� �����
   * ������������� ������� � ����������� ��������������
*/

#include <cstdlib>
#include <ctime>
#include "Enemy.h"
#include "CollisionSystem.h"

Enemy::Enemy()
{
    // ��������� ��������
    if (!texture.loadFromFile(Constants::RESOURCES_PATH + "enemy.png"))
    {
        throw std::runtime_error("Failed to load enemy texture!");
    }
    sprite.setTexture(texture);

    // ����������� ������
    float scale = Constants::PLAYER_SIZE / texture.getSize().x * 1.2f;
    sprite.setScale(scale, scale);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    speed = Constants::INIT_SPEED * 0.8f;
    changeDirectionTime = 1.5f + (rand() % 2000) / 1000.0f;
    direction = static_cast<Direction>(rand() % 4);
    updateRotation();
}

void Enemy::update(float deltaTime, const std::vector<std::unique_ptr<Obstacle>>& obstacles)
{
    // ����� ����������� �� �������
    if (directionTimer.getElapsedTime().asSeconds() > changeDirectionTime)
    {
        direction = static_cast<Direction>(rand() % 4);
        directionTimer.restart();
        changeDirectionTime = 1.0f + (rand() % 2000) / 1000.0f;
        updateRotation();
    }

    // ������������
    switch (direction)
    {
    case Direction::Right: position.x += speed * deltaTime; break;
    case Direction::Up:    position.y -= speed * deltaTime; break;
    case Direction::Left:  position.x -= speed * deltaTime; break;
    case Direction::Down:  position.y += speed * deltaTime; break;
    }

    // ����� �����������
    avoidObstacles(obstacles);
    checkBoundaries();
}

void Enemy::checkBoundaries()
{
    const float halfSize = Constants::PLAYER_SIZE / 2.0f;
    const float buffer = 5.0f; // �������� ���� � �����

    // ���� ������ � ���� - ������ �����������
    if (position.x < halfSize + buffer ||
        position.x > Constants::SCREEN_WIDTH - halfSize - buffer ||
        position.y < halfSize + buffer ||
        position.y > Constants::SCREEN_HEIGHT - halfSize - buffer)
    {
        if (static_cast<float>(rand()) / RAND_MAX < Constants::ENEMY_TURN_PROBABILITY)
        {
            direction = static_cast<Direction>(rand() % 4);
        }
    }

    // ����������� �������
    position.x = (position.x < halfSize) ? halfSize : (position.x > Constants::SCREEN_WIDTH - halfSize) ?
                  Constants::SCREEN_WIDTH - halfSize : position.x;
    position.y = (position.y < halfSize) ? halfSize : (position.y > Constants::SCREEN_WIDTH - halfSize) ?
                  Constants::SCREEN_HEIGHT - halfSize : position.y;
}

void Enemy::avoidObstacles(const std::vector<std::unique_ptr<Obstacle>>& obstacles)
{
    for (const auto& obs : obstacles)
    {
        if (Collision::circleRectCollision(*this, Constants::PLAYER_SIZE / 2, *obs, obs->getSize()))
        {
            // �������� ������ ����������� ��� ����������� � �����������
            direction = static_cast<Direction>(rand() % 4);
            break;
        }
    }
}

void Enemy::updateRotation()
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

void Enemy::draw(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Enemy::pauseTimers()
{
    savedTime = directionTimer.getElapsedTime();
}

void Enemy::resumeTimers()
{
    directionTimer.restart();
}

void Enemy::setColor(const sf::Color& color) 
{
    sprite.setColor(color);
}

sf::Color Enemy::getColor() const 
{
    return sprite.getColor();
}