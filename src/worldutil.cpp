#include "worldutil.hpp"

bool isOutside(const glm::ivec2& tile, GameData& data)
{
    return data.zones.zones.at(tile) == 0;
}
