/*
 _____ _   _ ________  ____   __
|  ___| \ | |  ___|  \/  \ \ / /
| |__ |  \| | |__ | .  . |\ V /
|  __|| . ` |  __|| |\/| | \ /
| |___| |\  | |___| |  | | | |
\____/\_| \_|____/\_|  |_/ \_/

����� Enemy ��������� ������ ��������� ������ � ����.
�������� ����������:
- ���������� ���������: ��������������, ����� ����������� ����� ������
- ����� ����������� (avoidObstacles)
- ���������� � �������� ���������: ���������, �������� ��������
- ��������� �������� � ����������� �������������

���������:
- ��������� ������:
  * ���������� ���������� (update/pause/resume)
  * �������������� � ���������� (checkBoundaries)
  * ������������ (draw)
  * ������� � ��������� ��������
- ��������� ����:
  * ����������� ���������� (shape)

����������� ����������:
- ������� �� � ��������� ������ �����������
- ��� �������� ������������ ����� getBounds() � FloatRect
- ������������ �����/������������� ���������� ��������
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObjects.h"
#include "Enums.h"
#include "Obstacle.h"

class Enemy : public GameObject
{
public:
    float speed;
    Direction direction;
    sf::RectangleShape shape;
    sf::Color color;
    sf::Clock directionTimer;
    sf::FloatRect getBounds() const override;
    sf::Color getColor() const;
    float changeDirectionTime;
    void checkBoundaries();
    void pauseTimers();
    void resumeTimers();
    void setColor(const sf::Color& color);
    
    Enemy();
    void update(float deltaTime, const std::vector<std::unique_ptr<Obstacle>>& obstacles);
    void draw(sf::RenderWindow& window) override;
    void avoidObstacles(const std::vector<std::unique_ptr<Obstacle>>& obstacles);

private:
    sf::Time savedTime;
    sf::Texture texture;
    sf::Sprite sprite;
    void updateRotation();
};
