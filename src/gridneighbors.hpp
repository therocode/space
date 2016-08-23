#pragma once
#include <cstdint>
#include <array>

template <typename T>
struct GridNeighbors
{
    std::array<std::pair<glm::ivec2, T*>, 4> neighbors;
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
            uint8_t count = 0;

            if(x > 0 && walls.atV({x, y}) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x - 1, y});
                result.at({x, y}).neighbors[count].first = {x - 1, y};
                ++count;
            }
            if(x < size.x - 1 && walls.atV({x + 1, y}) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x + 1, y});
                result.at({x, y}).neighbors[count].first = {x + 1, y};
                ++count;
            }
            if(y > 0 && walls.at({x, y}, Orientation::Horizontal) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x, y - 1});
                result.at({x, y}).neighbors[count].first = {x, y - 1};
                ++count;
            }
            if(y < size.y - 1 && walls.at({x, y + 1}, Orientation::Horizontal) == 0)
            {
                result.at({x, y}).neighbors[count].second = &grid.at({x, y + 1});
                result.at({x, y}).neighbors[count].first = {x, y + 1};
                ++count;
            }

            result.at({x, y}).neighborCount = count;
        }
    }

    return result;
}
