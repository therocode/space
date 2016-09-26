#pragma once
#include <thero/optional.hpp>
#include <data.hpp>
#include "actionutil.hpp"

ActionResult humanGoto(int32_t aiId, int32_t actionId, GameData& data);
ActionResult humanTotalPanic(int32_t aiId, int32_t actionId, GameData& data);
th::Optional<ActionCreateData> humanFindWorkTask(int32_t aiId, int32_t actionId, GameData& data);
ActionResult humanConstructWall(int32_t aiId, int32_t actionId, GameData& data);
ActionResult humanConstructDoor(int32_t aiId, int32_t actionId, GameData& data);
