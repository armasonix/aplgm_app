#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Apple;

class SpatialGrid 
{
public:
    SpatialGrid() = default;

    // Инициализация по размерам экрана и размеру ячейки
    void init(int screenWidth, int screenHeight, int cellSize);

    // Полная перестройка по всем активным яблокам
    void rebuild(const std::vector<std::unique_ptr<Apple>>& apples);

    // Инкрементальные операции вставка/удаление/перемещение одного яблока
    void insert(int appleIndex, const sf::Vector2f& pos);
    void erase(int appleIndex, const sf::Vector2f& pos);
    void move(int appleIndex, const sf::Vector2f& oldPos, const sf::Vector2f& newPos);

    // Собирает индексы яблок из ячейки игрока и 8 соседних 3x3
    void collectNear(const sf::Vector2f& pos, std::vector<int>& out) const;

    // Очищает сетку
    void clear();

private:
    int cellSize_ = 128;
    int cols_ = 0;
    int rows_ = 0;
    
    std::unordered_map<int, std::unordered_set<int>> cells_;

    inline int clampCol_(int c) const { return (c < 0 ? 0 : (c >= cols_ ? cols_ - 1 : c)); }
    inline int clampRow_(int r) const { return (r < 0 ? 0 : (r >= rows_ ? rows_ - 1 : r)); }
    int cellIndexFor(const sf::Vector2f& p) const;
};