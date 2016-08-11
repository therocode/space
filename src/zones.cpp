#include "zones.hpp"

void init(const glm::ivec2& size, int32_t id, Zones& zones)
{
    for(int32_t y = 0; y < size.y; ++y)
    {
        for(int32_t x = 0; x < size.x; ++x)
        {
            set({x, y}, id, zones);
        }
    }
}

void set(const glm::ivec2& coordinate, int32_t id, Zones& zones)
{
    int32_t old = at(coordinate, zones);

    if(old != id)
    {
        zones.zones.set(coordinate, id);

        zones.tiles[id].insert(coordinate);

        auto& coordinateSet = zones.tiles.at(old);
        coordinateSet.erase(coordinate);
        if(coordinateSet.empty())
            zones.tiles.erase(old);
    }
}

int32_t at(const glm::ivec2& coordinate, const Zones& zones)
{
    return zones.zones.at(coordinate);
}

const std::unordered_set<glm::ivec2>* tiles(int32_t id, const Zones& zones)
{
    auto found = zones.tiles.find(id);

    if(found == zones.tiles.end())
        return nullptr;
    else
        return &found->second;
}
