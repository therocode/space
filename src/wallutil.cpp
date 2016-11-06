#include "wallutil.hpp"
#include "doorutil.hpp"
#include <thero/assert.hpp>

glm::vec2 wallCenter(const WallPosition& position)
{
    glm::vec2 result = glm::vec2(position.position * 32);
    result += position.orientation == Orientation::Horizontal ? glm::vec2(16.0f, 0.0f) : glm::vec2(0.0f, 16.0f);
    return result;
}

WallPosition wallBetween(const glm::ivec2& a, const glm::ivec2& b)
{
    TH_ASSERT((a.x == b.x || a.y == b.y) && (std::abs(a.x - b.x) == 1 || std::abs(a.y - b.y) == 1), "invalid arguments " << a << " and " << b);

    if(a.x == b.x)
    {
        return {{a.x, std::max(a.y, b.y)}, Orientation::Horizontal};
    }
    else
    {
        return {{std::max(a.x, b.x), a.y}, Orientation::Vertical};
    }
}

glm::ivec2 otherSide(const WallPosition& position)
{
    return position.position + (position.orientation == Orientation::Vertical ? glm::ivec2(-1, 0) : glm::ivec2(0, -1));
}

th::Optional<WallPosition> positionToWall(const glm::vec2& position, float thickness)
{
    glm::ivec2 tile = position / 32.0f;
    glm::vec2 tileStart = tile * 32;
    glm::vec2 inTilePos = position - tileStart;
    float halfThickness = thickness / 2.0f;

    if(inTilePos.x < halfThickness)
        return WallPosition{tile, Orientation::Vertical};
    else if(inTilePos.x > 32.0f - halfThickness)
        return WallPosition{tile + glm::ivec2(1, 0), Orientation::Vertical};
    if(inTilePos.y < halfThickness)
        return WallPosition{tile, Orientation::Horizontal};
    else if(inTilePos.y > 32.0f - halfThickness)
        return WallPosition{tile + glm::ivec2(0, 1), Orientation::Horizontal};

    return {};
}

bool hasWall(const WallPosition& position, const GameData& data)
{
    return data.walls.at(position) != 0 && !hasDoor(position, data);
}

void setWall(const WallPosition& position, int32_t type, GameData& data)
{
    int32_t oldType = data.walls.at(position);

    if(oldType != type)
    {
        data.wallChanges[position] = {oldType, type};
    }
}
