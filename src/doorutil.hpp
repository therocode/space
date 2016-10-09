#pragma once
#include "data.hpp"
#include "wallmap.hpp"

void createDoor(Door door, GameData& data);
void toggleDoor(int32_t doorId, GameData& data);
void closeDoor(int32_t doorId, GameData& data);
void openDoor(int32_t doorId, GameData& data);
bool lockDoor(int32_t doorId, GameData& data);
void unlockDoor(int32_t doorId, GameData& data);
int64_t doorPressureDiff(int32_t doorId, GameData& data);
th::Optional<int32_t> lockedDoorAt(const WallPosition& position, GameData& data);
bool hasDoor(const WallPosition& position, GameData& data);
bool doorBordersZone(int32_t doorId, int32_t zoneId, GameData& data);
