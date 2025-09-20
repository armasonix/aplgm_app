/*
______  _____ _   _ _   _ _____    ___  ____________ _      _____
| ___ \|  _  | \ | | | | /  ___|  / _ \ | ___ \ ___ \ |    |  ___|
| |_/ /| | | |  \| | | | \ `--.  / /_\ \| |_/ / |_/ / |    | |__
| ___ \| | | | . ` | | | |`--. \ |  _  ||  __/|  __/| |    |  __|
| |_/ /\ \_/ / |\  | |_| /\__/ / | | | || |   | |   | |____| |___
\____/  \___/\_| \_/\___/\____/  \_| |_/\_|   \_|   \_____/\____/

Реализация методов класса BonusApple.

Основной функционал:
- Наследование функционала Apple с желтым цветом по умолчанию
- Реализация мигания через периодическое изменение цвета
- Контроль времени жизни через SFML Clock

Особенности реализации:
1. Циклическое переключение между желтым и фиолетовым цветами
2. Частота мигания: каждые 0.1 секунды
3. Время жизни определяется через Constants::BONUS_APPLE_DURATION
4. Наследование системы коллизий и позиционирования от Apple
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
