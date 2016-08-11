#pragma once
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "util/grid.hpp"
#include "glm.hpp"

struct Zones
{
    Grid<int32_t> zones;
    std::unordered_map<int32_t, std::unordered_set<glm::ivec2>> tiles;
};

void init(const glm::ivec2& size, int32_t id, Zones& zones);
void set(const glm::ivec2& coordinate, int32_t id, Zones& zones);
int32_t at(const glm::ivec2& coordinate, const Zones& zones);
const std::unordered_set<glm::ivec2>* tiles(int32_t id, const Zones& zones);
