#pragma once
#include <cstdint>
#include <array>
#include "util/grid.hpp"
#include "wallmap.hpp"

template <typename T>
struct GridNeighbors
{
    std::array<std::pair<size_t, T*>, 4> neighbors;
    uint8_t neighborCount;
};

template <typename T>
Grid<GridNeighbors<T>> findAllNeighbors(Grid<T>& grid, const WallMap& walls)
{
    Grid<GridNeighbors<T>> result(grid.size(), {});

    auto size = grid.size();
    for(int32_t y = 0; y < size.y; ++y)
    {
        for(int32_t x = 0; x < size.x; ++x)
        {
            size_t thisIndex = static_cast<size_t>(x + y * size.x);
            uint8_t count = 0;

            if(x > 0 && walls.atV({x, y}) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x - 1, y});
                result.at({x, y}).neighbors[count].first = thisIndex - 1;
                ++count;
            }
            if(x < size.x - 1 && walls.atV({x + 1, y}) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x + 1, y});
                result.at({x, y}).neighbors[count].first = thisIndex + 1;
                ++count;
            }
            if(y > 0 && walls.at({x, y}, Orientation::Horizontal) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x, y - 1});
                result.at({x, y}).neighbors[count].first = thisIndex - static_cast<size_t>(size.x);
                ++count;
            }
            if(y < size.y - 1 && walls.at({x, y + 1}, Orientation::Horizontal) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x, y + 1});
                result.at({x, y}).neighbors[count].first = thisIndex + static_cast<size_t>(size.x);
                ++count;
            }

            result.at({x, y}).neighborCount = count;
        }
    }

    return result;
}

template <typename T>
void updateNeighbors(Grid<GridNeighbors<T>>& neighbors, Grid<T>& grid, const WallMap& walls, const WallChanges& changes)
{
    auto size = grid.size();

    for(const auto& change : changes)
    {
        int32_t x = change.first.position.x;
        int32_t y = change.first.position.y;

        size_t thisIndex = static_cast<size_t>(x + y * size.x);

        uint8_t count = 0;

        if(x > 0 && walls.atV({x, y}) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x - 1, y});
            neighbors.at({x, y}).neighbors[count].first = thisIndex - 1;
            ++count;
        }
        if(x < size.x - 1 && walls.atV({x + 1, y}) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x + 1, y});
            neighbors.at({x, y}).neighbors[count].first = thisIndex + 1;
            ++count;
        }
        if(y > 0 && walls.at({x, y}, Orientation::Horizontal) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x, y - 1});
            neighbors.at({x, y}).neighbors[count].first = thisIndex - static_cast<size_t>(size.x);
            ++count;
        }
        if(y < size.y - 1 && walls.at({x, y + 1}, Orientation::Horizontal) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x, y + 1});
            neighbors.at({x, y}).neighbors[count].first = thisIndex + static_cast<size_t>(size.x);
            ++count;
        }

        neighbors.at({x, y}).neighborCount = count;

        if(change.first.orientation == Orientation::Vertical)
            --y;
        else
            --x;

        count = 0;

        if(x > 0 && walls.atV({x, y}) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x - 1, y});
            neighbors.at({x, y}).neighbors[count].first = thisIndex - 1;
            ++count;
        }
        if(x < size.x - 1 && walls.atV({x + 1, y}) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x + 1, y});
            neighbors.at({x, y}).neighbors[count].first = thisIndex + 1;
            ++count;
        }
        if(y > 0 && walls.at({x, y}, Orientation::Horizontal) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x, y - 1});
            neighbors.at({x, y}).neighbors[count].first = {thisIndex - static_cast<size_t>(size.x)};
            ++count;
        }
        if(y < size.y - 1 && walls.at({x, y + 1}, Orientation::Horizontal) == 0)
        {
            neighbors.at({x, y}).neighbors[count].second = &grid.at({x, y + 1});
            neighbors.at({x, y}).neighbors[count].first = {thisIndex + static_cast<size_t>(size.x)};
            ++count;
        }

        neighbors.at({x, y}).neighborCount = count;
    }
}
