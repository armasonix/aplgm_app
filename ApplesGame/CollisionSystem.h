/*
 _____ _____ _      _     _____ _____ _____ _____ _   _
/  __ \  _  | |    | |   |_   _/  ___|_   _|  _  | \ | |
| /  \/ | | | |    | |     | | \ `--.  | | | | | |  \| |
| |   | | | | |    | |     | |  `--. \ | | | | | | . ` |
| \__/\ \_/ / |____| |_____| |_/\__/ /_| |_\ \_/ / |\  |
 \____/\___/\_____/\_____/\___/\____/ \___/ \___/\_| \_/

������� ��������� ��������.

�������� ����������:
- �������� ������������ ����� ������� (circleCollide)
- �������� ������������ ����-������������� (circleRectCollision)

����������� ����������:
1. ������������� ��������� ���������� ��� ��������� ���������� ������
2. ������ � ��������� ����� ������� ����� GameObject
3. �������������� ������� � ������� �����������
*/

#pragma once
#include "GameObjects.h"

namespace Collision 
{
    inline bool circleCollide(const GameObject& a, const GameObject& b, float radiusA, float radiusB)
    {
        sf::Vector2f diff = a.position - b.position;
        float distanceSq = diff.x * diff.x + diff.y * diff.y;
        float radiusSum = radiusA + radiusB;
        return distanceSq <= (radiusSum * radiusSum);
    }

    inline bool circleRectCollision(const GameObject& circle, float radius, const GameObject& rect, const sf::Vector2f& size)
    {
        float closestX = std::max(rect.position.x, std::min(circle.position.x, rect.position.x + size.x));
        float closestY = std::max(rect.position.y, std::min(circle.position.y, rect.position.y + size.y));
        sf::Vector2f diff = circle.position - sf::Vector2f(closestX, closestY);
        return (diff.x * diff.x + diff.y * diff.y) < (radius * radius);
    }
}