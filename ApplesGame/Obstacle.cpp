/*
 ___________  _____ _____ ___  _____ _     _____
|  _  | ___ \/  ___|_   _/ _ \/  __ \ |   |  ___|
| | | | |_/ /\ `--.  | |/ /_\ \ /  \/ |   | |__
| | | | ___ \ `--. \ | ||  _  | |   | |   |  __|
\ \_/ / |_/ //\__/ / | || | | | \__/\ |___| |___
 \___/\____/ \____/  \_/\_| |_/\____|_____|____/

���������� ������� ������ Obstacle.

�������� ����������:
- ��������� �������������� ������ � ������� ���� (������)
- ����� draw() �������������� ������� � ����������� ��������������
- ������ getBounds() � getSize() �������� ����� SFML
- ���������� ������ �������������� ����� setColor / getColor

����������� ����������:
1. ���������������� ����� ���� position 
2. ������ � SFML-���������� RectangleShape
3. �������� ������������ ������������� � ������ ��������
*/

#include "Obstacle.h"

Obstacle::Obstacle(float width, float height) 
{
    shape.setSize({ width, height });
    shape.setFillColor(sf::Color::Yellow);
}

void Obstacle::draw(sf::RenderWindow& window) 
{
    shape.setPosition(position);
    window.draw(shape);
}

sf::FloatRect Obstacle::getBounds() const 
{
    return shape.getGlobalBounds();
}

sf::Vector2f Obstacle::getSize() const 
{
    return shape.getSize();
}

void Obstacle::setColor(const sf::Color& color) 
{
    shape.setFillColor(color);
}

sf::Color Obstacle::getColor() const 
{
    return shape.getFillColor();
}