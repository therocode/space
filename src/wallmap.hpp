#pragma once
#include <cstdint>
#include <vector>
#include "glm.hpp"
#include "orientation.hpp"

class WallMap
{
    public:
        WallMap(glm::ivec2 tileCount);
        int32_t at(const glm::ivec2& position, Orientation orientation) const;
        int32_t& at(const glm::ivec2& position, Orientation orientation);
        void set(const glm::ivec2& position, Orientation orientation, int32_t type);
        const glm::ivec2& size() const;
        const std::vector<int32_t>& horizontalWalls() const;
        const std::vector<int32_t>& verticalWalls() const;
        size_t toIndex(const glm::ivec2& position) const;
    private:
        glm::ivec2 mSize;
        std::vector<int32_t> mHorizontalWalls;
        std::vector<int32_t> mVerticalWalls;
};