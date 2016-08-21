#pragma once
#include "data.hpp"
#include "wallmap.hpp"

void closeDoor(int32_t doorId, wld::TDoor& tDoor, IdSet& openDoors, WallMap& walls);
