#pragma once
#include "data.hpp"
#include "wallmap.hpp"

void createDoor(Door door, GameData& data);
void toggleDoor(int32_t doorId, GameData& data);
void closeDoor(int32_t doorId, GameData& data);
void openDoor(int32_t doorId, GameData& data);
void lockDoor(int32_t doorId, GameData& data);
void unlockDoor(int32_t doorId, GameData& data);
