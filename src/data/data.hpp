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
#include "extract.hpp"
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

struct AssignedTask
{
    int32_t assigneeId;
};

struct BusyWorker
{
    int32_t taskId;
};

//http://members.shaw.ca/tfrisen/how_much_oxygen_for_a_person.htm
//one ingame hour: 20s. time ratio: 0.005555555555555
//need to use 2000 oxygene in 2 hours, 1000 in 1h, 1000/20 = 50 oxygene/s
struct BloodValues
{
    int32_t oxygen = 600; //500 is max, this will make them choke to death after 10 real life seconds
    bool dead = false; //temp
};

namespace ent
{
    using TPosition  = DataTable<glm::vec2>;
    using TPhysics  = DataTable<Physics>;
    using TWalkTarget = DataTable<glm::vec2>;
    using TMoveIntention = DataTable<MoveIntention>;
    using TMoveAbility = DataTable<MoveAbility>;
    using TBusyWorker = DataTable<BusyWorker>;
    using TBloodValues = DataTable<BloodValues>;
}

namespace tsk
{
    using TRoomTask = DataTable<RoomTask>;
    using TWallTask = DataTable<WallTask>;
    using TDoorTask = DataTable<DoorTask>;
    using TAssignedTask = DataTable<AssignedTask>;
}

namespace gfx
{
    using TActorSprite = DataTable<ActorSprite>;
}
