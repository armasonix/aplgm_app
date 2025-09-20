/*
 ___________  _____ _____ ___  _____ _     _____
|  _  | ___ \/  ___|_   _/ _ \/  __ \ |   |  ___|
| | | | |_/ /\ `--.  | |/ /_\ \ /  \/ |   | |__
| | | | ___ \ `--. \ | ||  _  | |   | |   |  __|
\ \_/ / |_/ //\__/ / | || | | | \__/\ |___| |___
 \___/\____/ \____/  \_/\_| |_/\____|_____|____/

 Класс Obstacle реализует логику игровых препятствий.

Основной функционал:
- Представление статических препятствий
- Обработка коллизий через bounding box (getBounds)
- Визуализация через SFML-примитив (прямоугольник)

Структура:
- Публичные методы:
  * Управление визуальными параметрами (setColor)
  * Получение геометрических характеристик (getSize)
  * Рендер объекта (draw)
- Публичные поля:
  * Графическое представление

Особенности реализации:
- Размеры задаются через конструкт
- Использует FloatRect для точного расчета коллизий
- Централизованное управление цветом через setColor
- Интеграция с системой рендеринга игры
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