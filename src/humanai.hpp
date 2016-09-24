#pragma once
#include <thero/optional.hpp>
#include <data.hpp>
#include "actionutil.hpp"

th::Optional<ActionVariant> humanGoto(int32_t aiId, int32_t actionId, GameData& data);
th::Optional<ActionVariant> humanTotalPanic(int32_t aiId, int32_t actionId, GameData& data);
th::Optional<ActionCreateData> humanFindWorkTask(int32_t aiId, int32_t actionId, GameData& data);
th::Optional<ActionVariant> humanConstructWall(int32_t aiId, int32_t actionId, GameData& data);
th::Optional<ActionVariant> humanConstructDoor(int32_t aiId, int32_t actionId, GameData& data);
