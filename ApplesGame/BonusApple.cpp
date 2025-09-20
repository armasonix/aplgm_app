/*
______  _____ _   _ _   _ _____    ___  ____________ _      _____
| ___ \|  _  | \ | | | | /  ___|  / _ \ | ___ \ ___ \ |    |  ___|
| |_/ /| | | |  \| | | | \ `--.  / /_\ \| |_/ / |_/ / |    | |__
| ___ \| | | | . ` | | | |`--. \ |  _  ||  __/|  __/| |    |  __|
| |_/ /\ \_/ / |\  | |_| /\__/ / | | | || |   | |   | |____| |___
\____/  \___/\_| \_/\___/\____/  \_| |_/\_|   \_|   \_____/\____/

���������� ������� ������ BonusApple.

�������� ����������:
- ������������ ����������� Apple � ������ ������ �� ���������
- ���������� ������� ����� ������������� ��������� �����
- �������� ������� ����� ����� SFML Clock

����������� ����������:
1. ����������� ������������ ����� ������ � ���������� �������
2. ������� �������: ������ 0.1 �������
3. ����� ����� ������������ ����� Constants::BONUS_APPLE_DURATION
4. ������������ ������� �������� � ���������������� �� Apple
*/

#include "BonusApple.h"

BonusApple::BonusApple() : Apple(sf::Color::Yellow) {}

void BonusApple::update() 
{
    if (blinkClock.getElapsedTime().asSeconds() >= 0.1f) 
    {
        blinkClock.restart();
        shape.setFillColor(shape.getFillColor() == sf::Color::Yellow
            ? sf::Color::Magenta : sf::Color::Yellow);
    }
}

bool BonusApple::isExpired() const 
{
    return lifeClock.getElapsedTime().asSeconds() >= Constants::BONUS_APPLE_DURATION;
}
