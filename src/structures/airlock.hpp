#pragma once
#include <cstdint>
#include <data.hpp>

void discoverAirlockDoors(int32_t id, const Structure& structure, GameData& data);
void requestMode(int32_t id, Airlock::Mode mode, GameData& data);
void startPumpDoor(int32_t id, Airlock::Mode targetMode, Airlock::Mode pumpMode, GameData& data);
void requestOpenAirlockDoor(int32_t id, int32_t doorId, GameData& data);
void airlockUpdate(GameData& data);
