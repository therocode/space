#pragma once
#include <data.hpp>

glm::vec2 tileCenter(glm::ivec2 tile);

template <typename GridType>
size_t toIndex(glm::ivec2 tile, const Grid<GridType>& grid)
{
    return static_cast<size_t>(tile.x + tile.y * grid.size().x);
}

template <typename GridType>
glm::ivec2 fromIndex(size_t index, Grid<GridType>& grid)
{
    return
    {
        static_cast<int32_t>(index) % grid.size().x,
        static_cast<int32_t>(index) / grid.size().y,
    };
}
