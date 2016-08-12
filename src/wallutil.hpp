#pragma once
#include <cstdint>
#include <vector>
#include "glm.hpp"
#include "wallmap.hpp"

struct WallChange
{
    glm::ivec2 position;
    Orientation orientation;
    int32_t value;
};

std::vector<WallChange> wallDiff(const WallMap& oldWalls, const WallMap& newWalls);
