#include "wallmap.hpp"
#include <thero/assert.hpp>

WallMap::WallMap(glm::ivec2 tileCount):
    mSize(tileCount + glm::ivec2(1, 1)),
    mHorizontalWalls(static_cast<size_t>(mSize.x * mSize.y), 0),
    mVerticalWalls(static_cast<size_t>(mSize.x * mSize.y), 0)
{
}

int32_t WallMap::at(const glm::ivec2& position, Orientation orientation) const
{
    TH_ASSERT(position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y, "Invalid coordinate " << position << " given to map of size " << mSize);
    size_t index = toIndex(position);
    return orientation == Orientation::Horizontal ?
        mHorizontalWalls[index] :
        mVerticalWalls[index];
}

int32_t& WallMap::at(const glm::ivec2& position, Orientation orientation)
{
    TH_ASSERT(position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y, "Invalid coordinate " << position << " given to map of size " << mSize);
    size_t index = toIndex(position);
    return orientation == Orientation::Horizontal ?
        mHorizontalWalls[index] :
        mVerticalWalls[index];
}

int32_t WallMap::atH(const glm::ivec2& position) const
{
    TH_ASSERT(position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y, "Invalid coordinate " << position << " given to map of size " << mSize);
    size_t index = toIndex(position);
    return mHorizontalWalls[index];
}

int32_t WallMap::atV(const glm::ivec2& position) const
{
    TH_ASSERT(position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y, "Invalid coordinate " << position << " given to map of size " << mSize);
    size_t index = toIndex(position);
    return mVerticalWalls[index];
}

int32_t WallMap::at(size_t index, Orientation orientation) const
{
    TH_ASSERT(index < mHorizontalWalls.size(), "Invalid index " << index << " given to map of size " << mSize);
    return orientation == Orientation::Horizontal ?
        mHorizontalWalls[index] :
        mVerticalWalls[index];
}

int32_t& WallMap::at(size_t index, Orientation orientation)
{
    TH_ASSERT(index < mHorizontalWalls.size(), "Invalid index " << index << " given to map of size " << mSize);
    return orientation == Orientation::Horizontal ?
        mHorizontalWalls[index] :
        mVerticalWalls[index];
}

int32_t WallMap::atH(size_t index) const
{
    TH_ASSERT(index < mHorizontalWalls.size(), "Invalid index " << index << " given to map of size " << mSize);
    return mHorizontalWalls[index];
}

int32_t WallMap::atV(size_t index) const
{
    TH_ASSERT(index < mHorizontalWalls.size(), "Invalid index " << index << " given to map of size " << mSize);
    return mVerticalWalls[index];
}

void WallMap::set(const glm::ivec2& position, Orientation orientation, int32_t type)
{
    TH_ASSERT(position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y, "Invalid coordinate " << position << " given to map of size " << mSize);
    if(orientation == Orientation::Horizontal)
        mHorizontalWalls[toIndex(position)] = type;
    else
        mVerticalWalls[toIndex(position)] = type;
}

void WallMap::fill(int32_t type)
{
    std::fill(mHorizontalWalls.begin(), mHorizontalWalls.end(), type);
    std::fill(mVerticalWalls.begin(), mVerticalWalls.end(), type);
}

const glm::ivec2& WallMap::size() const
{
    return mSize;
}

const std::vector<int32_t>& WallMap::horizontalWalls() const
{
    return mHorizontalWalls;
}

const std::vector<int32_t>& WallMap::verticalWalls() const
{
    return mVerticalWalls;
}

size_t WallMap::toIndex(const glm::ivec2& position) const
{
    return static_cast<size_t>(position.x + position.y * mSize.x);
}

void set(const WallPosition& position, int32_t type, const WallMap& walls, WallChanges& changes)
{
    int32_t oldType = walls.at(position.position, position.orientation);

    if(oldType != type)
    {
        changes[position] = {oldType, type};
    }
}
