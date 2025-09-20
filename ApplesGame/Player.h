/*
______ _      _____   _____________
| ___ \ |    / _ \ \ / /  ___| ___ \
| |_/ / |   / /_\ \ V /| |__ | |_/ /
|  __/| |   |  _  |\ / |  __||    /
| |   | |___| | | || | | |___| |\ \
\_|   \_____|_| |_/\_/ \____/\_| \_|

Класс Player реализует управление игровым персонажем.

Основной функционал:
- Управление движением в 4-х направлениях
- Система мигания при получении бонусов / урона
- Визуальное представление (текстура персонажа)

Структура:
- Приватные поля:
  * Графические параметры (texture, color)
  * Состояние движения (direction, speed)
  * Таймеры (blinkClock)
  * Флаги состояний (isBlinking)
- Публичные методы:
  * Управление состоянием (reset, update)
  * Визуализация (draw)
  * Управление цветом (setColor, getColor)
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObjects.h"
#include "Enums.h"

class Player : public GameObject 
{
private:
    sf::Texture texture;
    sf::Color baseColor;
    void updateRotation();

public:
    Direction direction;
    sf::Sprite sprite;
    sf::Clock blinkClock;
    sf::FloatRect getBounds() const override;
    sf::Color getColor() const;
    bool isBlinking = false;
    float speed;
    float getSpeed() const;

    Player();
    void reset();
    void update(float deltaTime);
    void updateBlink();
    void draw(sf::RenderWindow& window) override;
    void setColor(const sf::Color& color);
    void increaseSpeed();
    void resetSpeed();

};