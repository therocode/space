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

struct EntityData
{
    //game data
    ent::TPosition tPosition = {"Position", "The positions of game entities"};
    ent::TPhysics tPhysics = {"Physics", "The acceleration and velocities of game entities"};
    ent::TWalkTarget tWalkTarget = {"Walk Target", "The positions that game entities seek to move to"};
    ent::TMoveIntention tMoveIntention = {"Move Intention", "The directions and speeds that game entities which to attain"};
    ent::TMoveAbility tMoveAbility = {"Move Ability", "How well game entities are capable of moving"};
    ent::TBloodValues tBloodValues = {"Blood Values", "The content of vital compounds in the blood of an organism"};

    //worker stuff
    IdSet builders = {{}, {"Builders", "All workers who are builders"}};
    IdSet freeWorkers = {{}, {"Free workers", "All workers who are currently not working on any task"}};
    ent::TBusyWorker tBusyWorker = {"Busy Worker", "The workers who are working on a task"};
};

namespace tsk
{
    using TRoomTask = DataTable<RoomTask>;
    using TWallTask = DataTable<WallTask>;
    using TDoorTask = DataTable<DoorTask>;
    using TAssignedTask = DataTable<AssignedTask>;
}

struct TaskData 
{
    //tasks
    tsk::TRoomTask tRoomTask = {"Room Task", "Represent all rooms that need to be built"};
    tsk::TWallTask tWallTask = {"Wall Task", "Represent all walls that need to be built"};
    tsk::TDoorTask tDoorTask = {"Door Task", "Represent all doors that need to be built"};
    IdSet unassignedTasks = {{}, {"Unassigned tasks", "All tasks which no worker is currently on"}};
    tsk::TAssignedTask tAssignedTask = {"Assigned tasks", "All tasks which are assigned to a worker"};
};

namespace gfx
{
    using TActorSprite = DataTable<ActorSprite>;
}

struct GfxData
{
    //gfx
    gfx::TActorSprite tActorSprite = {"Actor Sprite", "Many-to-many relationship between game objects and sprites. Represents the visibility on screen of game entities"};
};
