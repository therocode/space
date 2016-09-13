#include "workerpathadaptor.hpp"
#include <thero/assert.hpp>

const glm::ivec2 up(0, -1);
const glm::ivec2 right(1, 0);
const glm::ivec2 down(0, 1);
const glm::ivec2 left(-1, 0);

WorkerPathAdaptor::WorkerPathAdaptor(const GameData& data) :
    mData(data),
    mWallSize(mData.walls.size())
{
}

const glm::ivec2 WorkerPathAdaptor::getNeighbor(const glm::ivec2& tile, uint32_t index) const
{
    uint32_t skip = index;

    if(tile.y > 0)
    {
        if(isPassable(tile, Orientation::Horizontal))
        {
            if(skip == 0)
                return tile + up;
            --skip;
        }
    }
    
    if(tile.x < mWallSize.x - 2)
    {
        if(isPassable(tile + right, Orientation::Vertical))
        {
            if(skip == 0)
                return tile + right;
            --skip;
        }
    }

    if(tile.y < mWallSize.y - 2)
    {
       if(isPassable(tile + down, Orientation::Horizontal))
       {
           if(skip == 0)
               return tile + down;
           --skip;
       }
    }
    
    if(tile.x > 0)
    {
        if(isPassable(tile, Orientation::Vertical))
        {
            if(skip == 0)
                return tile + left;
            --skip;
        }
    }

    TH_ASSERT(false, "Woops, bug in tile adaptor");

    return glm::ivec2();
}

uint32_t WorkerPathAdaptor::getNeighborAmount(const glm::ivec2& tile) const
{
    uint32_t amount = 0;

    if(tile.y > 0)
    {
        if(isPassable(tile, Orientation::Horizontal))
            ++amount;
    }
    
    if(tile.x < mWallSize.x - 2)
    {
        if(isPassable(tile + right, Orientation::Vertical))
            ++amount;
    }

    if(tile.y < mWallSize.y - 2)
    {
        if(isPassable(tile + down, Orientation::Horizontal))
            ++amount;
    }
    
    if(tile.x > 0)
    {
        if(isPassable(tile, Orientation::Vertical))
            ++amount;
    }

    return amount;
}

int32_t WorkerPathAdaptor::getStepCost(const glm::ivec2& tileA, const glm::ivec2& tileB) const
{
    return 1;
}

int32_t WorkerPathAdaptor::estimateDistance(const glm::ivec2& start, const glm::ivec2& target) const
{
    return static_cast<int32_t>(glm::distance((glm::vec2)start, (glm::vec2)target) * 10.0f);
}

bool WorkerPathAdaptor::isPassable(const glm::ivec2& tile, Orientation orientation) const
{
    bool Wall = mData.walls.at(tile, Orientation::Horizontal);

    if(wall)
    {
        auto door = findOne([&] (int32_t doorId, const Door& door)
        {
            return door.position == tile && door.orientation == orientation;   
        }, mData.tDoor);

        if(door)
        {
            bool unlockedDoor = !has(door->id, mData.lockedDoors);

            if(unlockedDoor)
            {
                return true;
            }
            else
            {//now it is a locked door, and a structure might be in control of it and might actually provide a path. We should check this here
                auto structureDoorLock = findOne([&] (const StructureDoorLock& lock)
                {
                    return door->id == lock.doorId;
                }, mData.tStructureDoorLock);

                if(structureDoorLock)
                {
                    th::Optional<int32_t> cost = structureAllowsDoorOpen(structureDoorLock->structureId, {tile, orientation}, mData);

                    if(cost)
                    {
                        return true;
                    }
                }
                //locked by other means...
            }
        }
    }

    return false;
}
