#pragma once
#include <cstdint>
#include <vector>
#include "common.hpp"
#include "join.hpp"
#include "insert.hpp"
#include "find.hpp"
#include "get.hpp"
#include "foreach.hpp"
#include "erase.hpp"
#include "clear.hpp"
#include "has.hpp"
//includes
#include "glm/glm.hpp"

//table layouts
struct ActionDuration
{
    ActionType action;
    int32_t duration;
};

using TActionDuration  = DataTable<ActionDuration>;
