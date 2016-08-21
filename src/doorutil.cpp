#include "doorutil.hpp"

void createDoor(Door door, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls)
{
    int32_t doorId = insert(std::move(door), tDoor);
    closeDoor(doorId, tDoor, openDoors, walls);
}

void toggleDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls)
{
    if(has(doorId, openDoors))
        closeDoor(doorId, tDoor, openDoors, walls);
    else
        openDoor(doorId, tDoor, openDoors, walls);
}

void closeDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls)
{
    const auto& door = get(doorId, tDoor);
    erase(doorId, openDoors);
    walls.set(door.position, door.orientation, 1);
}

void openDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls)
{
    const auto& door = get(doorId, tDoor);
    insert(doorId, openDoors);
    walls.set(door.position, door.orientation, 0);
}
