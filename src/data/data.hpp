#pragma once
#include <cstdint>
#include <vector>
#include "common.hpp"
#include "join.hpp"
#include "insert.hpp"
#include "find.hpp"
#include "get.hpp"
#include "set.hpp"
#include "foreach.hpp"
#include "erase.hpp"
#include "count.hpp"
#include "sort.hpp"
#include "clear.hpp"
#include "has.hpp"
#include "memory.hpp"
#include "accesspattern.hpp"
//includes
#include "glm/glm.hpp"
#include <fea/rendering/color.hpp>
#include "../orientation.hpp"

//table layouts
struct ActorSprite
{
    int32_t actorId;
    int32_t textureId;
    fea::Color color;
};

struct Physics
{
    glm::vec2 acceleration;
    glm::vec2 velocity;
};

struct MoveIntention
{
    glm::vec2 direction;
    float speedPercent;
};

struct MoveAbility
{
    float maxSpeed;
    float maxAcceleration;
};

struct RoomTask
{
    glm::ivec2 position;
    glm::ivec2 size;
};

struct WallTask
{
    glm::ivec2 position;
    Orientation orientation;
};

struct DoorTask
{
    glm::ivec2 position;
};

namespace ent
{
    using TPosition  = DataTable<glm::vec2>;
    using TPhysics  = DataTable<Physics>;
    using TWalkTarget = DataTable<glm::vec2>;
    using TMoveIntention = DataTable<MoveIntention>;
    using TMoveAbility = DataTable<MoveAbility>;
}

namespace tsk
{
    using TRoomTask = DataTable<RoomTask>;
    using TWallTask = DataTable<WallTask>;
    using TDoorTask = DataTable<DoorTask>;
}

namespace gfx
{
    using TActorSprite = DataTable<ActorSprite>;
}
