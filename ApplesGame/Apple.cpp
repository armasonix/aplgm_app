/*
  ___  ____________ _      _____
 / _ \ | ___ \ ___ \ |    |  ___|
/ /_\ \| |_/ / |_/ / |    | |__
|  _  ||  __/|  __/| |    |  __|
| | | || |   | |   | |____| |___
\_| |_/\_|   \_|   \_____/\____/

���������� ������� ������ Apple.

�������� ����������:
- ������������� ������� shape � GameObject::position
- ��������� ������ ��� �������� �������� (active = true)
- ������ �������� ����� ���������� ������� �����

����������� ����������:
1. ������ �������� ����� Constants::APPLE_SIZE
2. ������������� origin ��� ����������� ����������������
3. �������������� ������������� ������� � ������ draw()
4. ������ ����������� ����� �� ��������� ������������
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