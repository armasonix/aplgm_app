/*
  ___  ____________ _      _____
 / _ \ | ___ \ ___ \ |    |  ___|
/ /_\ \| |_/ / |_/ / |    | |__
|  _  ||  __/|  __/| |    |  __|
| | | || |   | |   | |____| |___
\_| |_/\_|   \_|   \_____/\____/

����� Apple ��������� ������� �������-������.

�������� ����������:
- ���������� ������������� ����� SFML-�������� (����)
- ���������� ����������� ������� (���� active)
- ���������������� ���������� ������ ����� setColor

���������:
- ��������� ������:
  * ���������� �������� (setColor, draw)
  * ��������� ������������� (getBounds, getColor)
- ��������� ����:
  * ������ ���������� (active)
  * ����������� ����� (shape)

����������� ����������:
- ������ �������� ����� Constants.h (APPLE_SIZE)
- ���������� CircleShape ��� �������������
- ����������� ����: ������� (������������� ����� �����������)
- �������� �������������� ����� bounding box
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