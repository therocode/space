#pragma once

template <typename Functor>
void forEachWall(const glm::ivec2& start, const glm::ivec2& size, Functor f)
{
    for(int32_t x = start.x; x < start.x + size.x; ++x)
    {
        f({x, start.y}, Orientation::Horizontal);
        f({x, start.y + size.y}, Orientation::Horizontal);
    }
    for(int32_t y = start.y; y < start.y + size.y; ++y)
    {
        f({start.x, y}, Orientation::Vertical);
        f({start.x + size.x, y}, Orientation::Vertical);
    }
}

template <typename Functor>
void forEachFloor(const glm::ivec2& start, const glm::ivec2& size, Functor f)
{
    for(int32_t y = start.y; y < start.y + size.y; ++y)
    {
        for(int32_t x = start.x; x < start.x + size.x; ++x)
        {
            f({x, y});
        }
    }
}
