#include "doorutil.hpp"

void createDoor(Door door, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes)
{
    int32_t doorId = insert(std::move(door), tDoor);
    closeDoor(doorId, tDoor, openDoors, walls, changes);
}

void toggleDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes)
{
    if(has(doorId, openDoors))
        closeDoor(doorId, tDoor, openDoors, walls, changes);
    else
        openDoor(doorId, tDoor, openDoors, walls, changes);
}

void closeDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes)
{
    const auto& door = get(doorId, tDoor);
    erase(doorId, openDoors);
    set({door.position, door.orientation}, 1, walls, changes);
}

void openDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes)
{
    const auto& door = get(doorId, tDoor);
    insert(doorId, openDoors);
    set({door.position, door.orientation}, 0, walls, changes);
}
