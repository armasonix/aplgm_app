/*
  ___  ____________ _      _____
 / _ \ | ___ \ ___ \ |    |  ___|
/ /_\ \| |_/ / |_/ / |    | |__
|  _  ||  __/|  __/| |    |  __|
| | | || |   | |   | |____| |___
\_| |_/\_|   \_|   \_____/\____/

Класс Apple реализует игровые объекты-яблоки.

Основной функционал:
- Визуальное представление через SFML-примитив (круг)
- Управление активностью объекта (флаг active)
- Централизованное управление цветом через setColor

Структура:
- Публичные методы:
  * Управление графикой (setColor, draw)
  * Получение характеристик (getBounds, getColor)
- Публичные поля:
  * Статус активности (active)
  * Графическая форма (shape)

Особенности реализации:
- Размер задается через Constants.h (APPLE_SIZE)
- Использует CircleShape для представления
- Изначальный цвет: красный (настраивается через конструктор)
- Коллизии рассчитываются через bounding box
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "Constants.h"

class Apple : public GameObject 
{
public:
    sf::CircleShape shape;
    sf::FloatRect getBounds() const override;
    sf::Color getColor() const;
    bool active = true;

    Apple(const sf::Color& color = sf::Color::Red);
    void draw(sf::RenderWindow& window) override;
    void setColor(const sf::Color& color);
    
};