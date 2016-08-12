#include "wallpathadaptor.hpp"
#include <thero/assert.hpp>

WallPathAdaptor::WallPathAdaptor(const WallMap& walls) :
    mWalls(walls)
{
}

const glm::ivec2 WallPathAdaptor::getNeighbor(const glm::ivec2& tile, uint32_t index) const
{
    uint32_t skip = index;

    glm::ivec2 up(0, -1);
    if(tile.y > 0 && !mWalls.at(tile, Orientation::Horizontal))
    {
        if(skip == 0)
            return tile + up;
        --skip;
    }
    
    glm::ivec2 right(1, 0);
    if(tile.x < mWalls.size().x - 2 && !mWalls.at(tile + glm::ivec2(1, 0), Orientation::Vertical))
    {
        if(skip == 0)
            return tile + right;
        --skip;
    }

    glm::ivec2 down(0, 1);
    if(tile.y < mWalls.size().y - 2 && !mWalls.at(tile + down, Orientation::Horizontal))
    {
        if(skip == 0)
            return tile + down;
        --skip;
    }
    
    glm::ivec2 left(-1, 0);
    if(tile.x > 0 && !mWalls.at(tile, Orientation::Vertical))
    {
        if(skip == 0)
            return tile + left;
        --skip;
    }

    TH_ASSERT(false, "Woops, bug in tile adaptor");

    return glm::ivec2();
}

uint32_t WallPathAdaptor::getNeighborAmount(const glm::ivec2& tile) const
{
    uint32_t amount = 0;

    glm::ivec2 up(0, -1);
    if(tile.y > 0 && !mWalls.at(tile, Orientation::Horizontal))
        ++amount;
    
    glm::ivec2 right(1, 0);
    if(tile.x < mWalls.size().x - 2 && !mWalls.at(tile + glm::ivec2(1, 0), Orientation::Vertical))
        ++amount;

    glm::ivec2 down(0, 1);
    if(tile.y < mWalls.size().y - 2 && !mWalls.at(tile + down, Orientation::Horizontal))
        ++amount;
    
    glm::ivec2 left(-1, 0);
    if(tile.x > 0 && !mWalls.at(tile, Orientation::Vertical))
        ++amount;

    return amount;
}

int32_t WallPathAdaptor::getStepCost(const glm::ivec2& tileA, const glm::ivec2& tileB) const
{
    return 1;
}

int32_t WallPathAdaptor::estimateDistance(const glm::ivec2& start, const glm::ivec2& target) const
{
    return static_cast<int32_t>(glm::distance((glm::vec2)start, (glm::vec2)target) * 10.0f);
}
