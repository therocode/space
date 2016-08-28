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

struct CollisionBox
{
    glm::vec2 size;
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
    Orientation orientation;
};

struct AssignedTask
{
    int32_t assigneeId;
};

struct BusyWorker
{
    int32_t taskId;
};

struct Choking
{
    float breathingCapability;
};

struct Door
{
    glm::ivec2 position;
    Orientation orientation;
};

//http://members.shaw.ca/tfrisen/how_much_oxygen_for_a_person.htm
//one ingame hour: 20s. time ratio: 0.005555555555555
//need to use 2000 oxygene in 2 hours, 1000 in 1h, 1000/20 = 50 oxygene/s
struct BloodValues
{
    int32_t oxygen = 600; //500 is max, this will make them choke to death after 10 real life seconds
};

struct StructureType
{
    std::string name;
    int32_t texture;
};

struct Structure
{
    glm::ivec2 position;
    int32_t structureType;
};

//entity
using TPosition  = DataTable<glm::vec2, false>;
using TPhysics  = DataTable<Physics, true>;
using TCollisionBox  = DataTable<CollisionBox, true>;
using TWalkTarget = DataTable<glm::vec2, true>;
using TMoveIntention = DataTable<MoveIntention, true>;
using TMoveAbility = DataTable<MoveAbility, true>;
using TBusyWorker = DataTable<BusyWorker, true>;
using TBloodValues = DataTable<BloodValues, true>;
using TChoking = DataTable<Choking, true>;
//tasks
using TRoomTask = DataTable<RoomTask, true>;
using TWallTask = DataTable<WallTask, true>;
using TDoorTask = DataTable<DoorTask, true>;
using TAssignedTask = DataTable<AssignedTask, true>;
//world
using TDoor = DataTable<Door, false>;
//structure
using TStructure = DataTable<Structure, false>;
using TStructureType = DataTable<StructureType, true>;
using TActorSprite = DataTable<ActorSprite, false>;

struct GameData
{
    //game data
    TPosition tPosition = {"Position", "The positions of game entities"};
    TPhysics tPhysics = {"Physics", "The acceleration and velocities of game entities"};
    TCollisionBox tCollisionBox = {"Collision Box", "The box used to calculate the physical presence of entities"};
    TWalkTarget tWalkTarget = {"Walk Target", "The positions that game entities seek to move to"};
    TMoveIntention tMoveIntention = {"Move Intention", "The directions and speeds that game entities which to attain"};
    TMoveAbility tMoveAbility = {"Move Ability", "How well game entities are capable of moving"};

    //worker stuff
    IdSet builders = {{}, {"Builders", "All workers who are builders"}};
    IdSet freeWorkers = {{}, {"Free workers", "All workers who are currently not working on any task"}};
    IdSet deadWorkers = {{}, {"Dead workers", "All workers who are no longer alive"}};
    TBusyWorker tBusyWorker = {"Busy Worker", "The workers who are working on a task"};

    //organism stuff
    TBloodValues tBloodValues = {"Blood Values", "The content of vital compounds in the blood of an organism"};
    TChoking tChoking = {"Choking", "Lists the impaired breathing capabilities of organisms that are currently not able to absorb oxygen to 100%"};
    IdSet died = {{}, {"Died", "The organisms that have just died, will only be set for one frame"}};
    
    //tasks
    TRoomTask tRoomTask = {"Room Task", "Represents all rooms that need to be built"};
    TWallTask tWallTask = {"Wall Task", "Represents all walls that need to be built"};
    TDoorTask tDoorTask = {"Door Task", "Represents all doors that need to be built"};
    IdSet unassignedTasks = {{}, {"Unassigned tasks", "All tasks which no worker is currently on"}};
    TAssignedTask tAssignedTask = {"Assigned tasks", "All tasks which are assigned to a worker"};

    //world
    TDoor tDoor = {"Door", "Represents all doors"};
    IdSet openDoors = {{}, {"Open doors", "All doors that are open"}};

    //structure
    TStructure tStructure = {"Structure", "All existing structures"};
    TStructureType tStructureType = {"Structure Type", "All types of structures"};

    //gfx
    ::TActorSprite tActorSprite = {"Actor Sprite", "Many-to-many relationship between game objects and sprites. Represents the visibility on screen of game entities"};
};
