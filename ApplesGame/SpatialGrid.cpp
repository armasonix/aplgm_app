#include "SpatialGrid.h"
#include "Apple.h"
#include "Constants.h"
#include <algorithm>

void SpatialGrid::init(int screenWidth, int screenHeight, int cellSize) 
{
    cellSize_ = cellSize > 0 ? cellSize : 128;
    cols_ = (screenWidth + cellSize_ - 1) / cellSize_;
    rows_ = (screenHeight + cellSize_ - 1) / cellSize_;
    cells_.clear();
}

void SpatialGrid::clear() 
{
    cells_.clear();
}

int SpatialGrid::cellIndexFor(const sf::Vector2f& p) const 
{
    const int col = clampCol_(static_cast<int>(p.x) / cellSize_);
    const int row = clampRow_(static_cast<int>(p.y) / cellSize_);
    return row * cols_ + col;
}

void SpatialGrid::insert(int appleIndex, const sf::Vector2f& pos) 
{
    const int idx = cellIndexFor(pos);
    cells_[idx].insert(appleIndex);
}

void SpatialGrid::erase(int appleIndex, const sf::Vector2f& pos) 
{
    const int idx = cellIndexFor(pos);
    auto it = cells_.find(idx);
    if (it != cells_.end()) 
    {
        it->second.erase(appleIndex);
        if (it->second.empty()) cells_.erase(it);
    }
}

void SpatialGrid::move(int appleIndex, const sf::Vector2f& oldPos, 
    const sf::Vector2f& newPos) 
{
    const int oldIdx = cellIndexFor(oldPos);
    const int newIdx = cellIndexFor(newPos);
    if (oldIdx == newIdx) return;
    erase(appleIndex, oldPos);
    insert(appleIndex, newPos);
}

void SpatialGrid::rebuild(const std::vector<std::unique_ptr<Apple>>& apples) 
{
    // если init() не вызывалс€ Ч инициализирует из Constants
    if (cols_ == 0 || rows_ == 0) 
    {
        init(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, Constants::GRID_CELL_SIZE);
    }
    cells_.clear();
    for (int i = 0; i < static_cast<int>(apples.size()); ++i) 
    {
        const auto& a = apples[i];
        if (!a || !a->active) continue;
        insert(i, a->position);
    }
}

void SpatialGrid::collectNear(const sf::Vector2f& pos, std::vector<int>& out) const 
{
    out.clear();
    if (cols_ == 0 || rows_ == 0) return;

    const int col = clampCol_(static_cast<int>(pos.x) / cellSize_);
    const int row = clampRow_(static_cast<int>(pos.y) / cellSize_);

    for (int dr = -1; dr <= 1; ++dr) 
    {
        for (int dc = -1; dc <= 1; ++dc) 
        {
            const int rr = row + dr;
            const int cc = col + dc;
            if (rr < 0 || rr >= rows_ || cc < 0 || cc >= cols_) continue;
            const int idx = rr * cols_ + cc;
            auto it = cells_.find(idx);
            if (it == cells_.end()) continue;
            out.insert(out.end(), it->second.begin(), it->second.end());
        }
    }
}