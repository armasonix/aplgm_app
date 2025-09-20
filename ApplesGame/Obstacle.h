/*
 ___________  _____ _____ ___  _____ _     _____
|  _  | ___ \/  ___|_   _/ _ \/  __ \ |   |  ___|
| | | | |_/ /\ `--.  | |/ /_\ \ /  \/ |   | |__
| | | | ___ \ `--. \ | ||  _  | |   | |   |  __|
\ \_/ / |_/ //\__/ / | || | | | \__/\ |___| |___
 \___/\____/ \____/  \_/\_| |_/\____|_____|____/

 ����� Obstacle ��������� ������ ������� �����������.

�������� ����������:
- ������������� ����������� �����������
- ��������� �������� ����� bounding box (getBounds)
- ������������ ����� SFML-�������� (�������������)

���������:
- ��������� ������:
  * ���������� ����������� ����������� (setColor)
  * ��������� �������������� ������������� (getSize)
  * ������ ������� (draw)
- ��������� ����:
  * ����������� �������������

����������� ����������:
- ������� �������� ����� ���������
- ���������� FloatRect ��� ������� ������� ��������
- ���������������� ���������� ������ ����� setColor
- ���������� � �������� ���������� ����
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "GameObjects.h"

class Obstacle : public GameObject
{
public:
    sf::RectangleShape shape;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getSize() const;
    sf::Color getColor() const;

    Obstacle(float width, float height);
    void draw(sf::RenderWindow& window) override;
    void setColor(const sf::Color& color); 
};