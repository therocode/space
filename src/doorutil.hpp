#pragma once
#include "data.hpp"
#include "wallmap.hpp"

void createDoor(Door door, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls);
void toggleDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls);
void closeDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls);
void openDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls);
