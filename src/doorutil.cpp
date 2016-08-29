#include "doorutil.hpp"

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
    if(has(doorId, data.openDoors))
        closeDoor(doorId, data);
    insert(doorId, data.lockedDoors);
}

void unlockDoor(int32_t doorId, GameData& data)
{
    erase(doorId, data.lockedDoors);
}
