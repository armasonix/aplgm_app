/*
  ___  ____________ _      _____
 / _ \ | ___ \ ___ \ |    |  ___|
/ /_\ \| |_/ / |_/ / |    | |__
|  _  ||  __/|  __/| |    |  __|
| | | || |   | |   | |____| |___
\_| |_/\_|   \_|   \_____/\____/

Реализация методов класса Apple.

Основной функционал:
- Синхронизация позиции shape с GameObject::position
- Отрисовка только для активных объектов (active = true)
- Расчет коллизий через глобальные границы формы

Особенности реализации:
1. Размер задается через Constants::APPLE_SIZE
2. Центрирование origin для корректного позиционирования
3. Автоматическая синхронизация позиции в методе draw()
4. Прямая зависимость цвета от параметра конструктора
*/

#include "Apple.h"


Apple::Apple(const sf::Color& color)
{
    shape.setRadius(Constants::APPLE_SIZE / 2);
    shape.setOrigin(Constants::APPLE_SIZE / 2, Constants::APPLE_SIZE / 2);
    shape.setFillColor(color);
}

void Apple::draw(sf::RenderWindow& window)
{
    if (active)
    {
        shape.setPosition(position);
        window.draw(shape);
    }
}

sf::FloatRect Apple::getBounds() const
{
    return shape.getGlobalBounds();
}

void Apple::setColor(const sf::Color& color) 
{
    shape.setFillColor(color);
}

sf::Color Apple::getColor() const 
{
    return shape.getFillColor();
}