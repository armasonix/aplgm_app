/*
______ _      _____   _____________
| ___ \ |    / _ \ \ / /  ___| ___ \
| |_/ / |   / /_\ \ V /| |__ | |_/ /
|  __/| |   |  _  |\ / |  __||    /
| |   | |___| | | || | | |___| |\ \
\_|   \_____|_| |_/\_/ \____/\_| \_|

����� Player ��������� ���������� ������� ����������.

�������� ����������:
- ���������� ��������� � 4-� ������������
- ������� ������� ��� ��������� ������� / �����
- ���������� ������������� (�������� ���������)

���������:
- ��������� ����:
  * ����������� ��������� (texture, color)
  * ��������� �������� (direction, speed)
  * ������� (blinkClock)
  * ����� ��������� (isBlinking)
- ��������� ������:
  * ���������� ���������� (reset, update)
  * ������������ (draw)
  * ���������� ������ (setColor, getColor)
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