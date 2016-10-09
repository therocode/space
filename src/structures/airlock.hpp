#pragma once
#include <cstdint>
#include <data.hpp>

void discoverAirlockDoors(int32_t id, const Structure& structure, GameData& data);
void autoDetectAirlockMode(int32_t id, GameData& data);
void requestMode(int32_t id, Airlock::Mode mode, GameData& data);
void startPumpDoor(int32_t id, Airlock::Mode targetMode, Airlock::Mode pumpMode, GameData& data);
void requestOpenAirlockDoor(int32_t id, int32_t doorId, GameData& data);
void airlockUpdate(GameData& data);
th::Optional<int32_t> airlockProvidesPath(int32_t structureId, const WallPosition& wallPosition, const GameData& data);
