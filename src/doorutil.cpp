#include "doorutil.hpp"

void closeDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls)
{
    const auto& door = get(doorId, tDoor);
    erase(doorId, openDoors);
    walls.set(door.position, door.orientation, 1);
}
