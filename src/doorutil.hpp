#pragma once
#include "data.hpp"
#include "wallmap.hpp"

void createDoor(Door door, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes);
void toggleDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes);
void closeDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes);
void openDoor(int32_t doorId, TDoor& tDoor, IdSet& openDoors, const WallMap& walls, WallChanges& changes);
