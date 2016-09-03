#include "doorutil.hpp"
#include "gases.hpp"

void createDoor(Door door, GameData& data)
{
    int32_t doorId = insert(std::move(door), data.tDoor);
    closeDoor(doorId, data);
}

void toggleDoor(int32_t doorId, GameData& data)
{
    if(has(doorId, data.openDoors))
        closeDoor(doorId, data);
    else
        openDoor(doorId, data);
}

void closeDoor(int32_t doorId, GameData& data)
{
    const auto& door = get(doorId, data.tDoor);
    erase(doorId, data.openDoors);
    set({door.position, door.orientation}, 1, data.walls, data.wallChanges);
}

void openDoor(int32_t doorId, GameData& data)
{
    if(!has(doorId, data.lockedDoors))
    {
        const auto& door = get(doorId, data.tDoor);
        insert(doorId, data.openDoors);
        set({door.position, door.orientation}, 0, data.walls, data.wallChanges);
    }
}

void lockDoor(int32_t doorId, GameData& data)
{
    if(!has(doorId, data.lockedDoors))
    {
        if(has(doorId, data.openDoors))
            closeDoor(doorId, data);
        insert(doorId, data.lockedDoors);
    }
}

void unlockDoor(int32_t doorId, GameData& data)
{
    erase(doorId, data.lockedDoors);
}

int64_t doorPressureDiff(int32_t doorId, GameData& data)
{
    const Door& door = get(doorId, data.tDoor);
    const glm::ivec2& start = door.position;
    const glm::ivec2& end = start + (door.orientation == Orientation::Horizontal ? glm::ivec2(0, -1) : glm::ivec2(-1, 0));

    int64_t startPressure = pressure(data.atmosphere.at(start));
    int64_t endPressure = pressure(data.atmosphere.at(end));

    return startPressure - endPressure;
}

glm::ivec2 otherSide(const Door& door)
{
    return door.position + (door.orientation == Orientation::Vertical ? glm::ivec2(-1, 0) : glm::ivec2(0, -1));
}
