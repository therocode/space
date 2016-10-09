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
#include "../workerpathadaptor.hpp"
#include "../orientation.hpp"
#include "../wallmap.hpp"
#include "../gases.hpp"
#include "../zones.hpp"
#include "../gridneighbors.hpp"

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

//ai
struct Ai
{
    enum Type { Human };
    Type type;
};

//tasks
struct Task
{
    enum Type {Wall, Door, Room};
    int32_t priority;
    Type type;
};

struct RoomTask
{
    glm::ivec2 position;
    glm::ivec2 size;
    static constexpr Task::Type type = Task::Room;
};

struct WallTask
{
    WallPosition position;
    static constexpr Task::Type type = Task::Wall;
};

struct DoorTask
{
    WallPosition position;
    static constexpr Task::Type type = Task::Door;
};

struct AssignedTask
{
    int32_t assigneeId;
};

struct Item
{
    enum Type { SpaceSuit };
    Type type;
    int32_t weight; //in grams
};

struct Wearable
{
    th::Optional<int32_t> wearer;
    th::Optional<int32_t> airTank;
};

struct ItemContainer
{
};

struct ItemStoring
{
    int32_t itemId;
    int32_t containerId;
};

struct AirTank
{
    int32_t capacity;
    Gases gases;
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
    WallPosition position;
};

struct StructureDoorLock
{
    int32_t doorId;
    int32_t structureId;
};

struct ZoneLeak
{
    glm::ivec2 start;
    glm::ivec2 end;
    int32_t size;
    int32_t pumpForce;
};

struct Airlock
{
    enum Mode { In, Out, Pumping};
    Mode currentMode;
    int32_t coolDown = 0;
    std::vector<int32_t> doors;
    th::Optional<int32_t> exit;
};

struct Crate
{
    int32_t containerId;
};

struct AirlockActivity
{
    Airlock::Mode targetMode;
    Airlock::Mode currentlyPumping;
    int32_t leakId;
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
    enum Type {Airlock, CryoPods, Battery, Crate, Toilet};
    glm::ivec2 position;
    Type type;
};

struct Incentive
{
    enum Type {Breathe, Work};
    int32_t actorId;
    int32_t importance;
    Type type;
};

struct BreatheIncentive
{
};

struct WorkIncentive
{
};

struct Action
{
    enum Type {Goto, EquipSpaceSuit, TotalPanic, FindWorkTask, ConstructWall, ConstructDoor};
    int32_t actorId;
    th::Optional<int32_t> parentAction;
    Type type;
};

struct TaskAction
{
    int32_t actionId;
    int32_t taskId;
};

struct GotoAction
{
    enum AllowUnbreathable { Allow, Disallow };
    glm::vec2 target;
    float acceptableDistance;
    th::Optional<int32_t> pathId;
    th::Optional<size_t> pathIndex;
    AllowUnbreathable allowUnbreathable;
    static constexpr Action::Type type = Action::Goto;
};

struct EquipSpaceSuitAction
{
    static constexpr Action::Type type = Action::EquipSpaceSuit;
};

struct TotalPanicAction
{
    static constexpr Action::Type type = Action::TotalPanic;
};

struct FindWorkTaskAction
{
    static constexpr Action::Type type = Action::FindWorkTask;
};

struct ConstructWallAction
{
    static constexpr Action::Type type = Action::ConstructWall;
    int32_t workLeft;
};

struct ConstructDoorAction
{
    static constexpr Action::Type type = Action::ConstructDoor;
    int32_t workLeft;
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
//AI
using TAi = DataTable<Ai, true>;
using TIncentive = DataTable<Incentive, false>;
using TBreatheIncentive = DataTable<BreatheIncentive, true>;
using TWorkIncentive = DataTable<WorkIncentive, true>;
//paths
using Path = fea::Pathfinder<WorkerPathAdaptor>::Result;
using TPath = DataTable<Path, false>;
//action
using TAction = DataTable<Action, false>;
using TTaskAction = DataTable<TaskAction, false>;
using TGotoAction = DataTable<GotoAction, true>;
using TEquipSpaceSuitAction = DataTable<EquipSpaceSuitAction, true>; //np
using TTotalPanicAction = DataTable<TotalPanicAction, true>;
using TFindWorkTaskAction = DataTable<FindWorkTaskAction, true>;
using TConstructWallAction = DataTable<ConstructWallAction, true>;
using TConstructDoorAction = DataTable<ConstructDoorAction, true>;
//tasks
using TTask = DataTable<Task, false>;
using TRoomTask = DataTable<RoomTask, true>;
using TWallTask = DataTable<WallTask, true>;
using TDoorTask = DataTable<DoorTask, true>;
using TAssignedTask = DataTable<AssignedTask, true>;
//items
using TItem = DataTable<Item, false>;
using TWearable = DataTable<Wearable, true>;
//containers
using TItemContainer = DataTable<ItemContainer, false>;
using TItemStoring = DataTable<ItemStoring, false>;
using TAirTank = DataTable<AirTank, false>;
//world
using TDoor = DataTable<Door, false>;
using TStructureDoorLock = DataTable<StructureDoorLock, false>;
using TZoneLeak = DataTable<ZoneLeak, false>;
//structure
using TStructure = DataTable<Structure, false>;
using TStructureType = DataTable<StructureType, true>;
using TAirlock = DataTable<Airlock, true>;
using TAirlockActivity = DataTable<AirlockActivity, true>;
using TCrate = DataTable<Crate, true>;
//gfx
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

    //ai
    TAi tAi = {"AI", "Agents that can come to decisions"};
    IdSet uninitializedAis = {{}, {"Uninitialized AIs", "AI agents that are in need of initialization"}};
    IdSet humanAis = {{}, {"Human AIs", "AI agents that think like a human"}};
    TIncentive tIncentive = {"Incentive", "All incentives that drive actions in AI agents"};
    TBreatheIncentive tBreatheIncentive = {"Breathe Incentive", "Makes creatures want to breathe"};
    TWorkIncentive tWorkIncentive = {"Work Incentive", "Makes creatures want to work"};
    IdSet activeIncentives = {{}, {"Active Incentives", "The incentives that are the ones currently acted upon"}};
    
    //path
    TPath tPath = {"Path", "Path cache"};

    //actions
    TAction tAction = {"Action", "The currently existing actions for all AI agents"};
    IdSet leafActions = {{}, {"Leaf actions", "Actions that have no children."}};
    TTaskAction tTaskAction = {"Task Action", "Actions that are for a task"};
    TGotoAction tGotoAction = {"Goto action", "Current goto actions"};
    TEquipSpaceSuitAction tEquipSpaceSuitAction = {"Equip Space Suit action", "Current actions to obtain and equip space suits"};
    TTotalPanicAction tTotalPanicAction = {"Total Panic Action", "Current total panic actions"};
    TFindWorkTaskAction tFindWorkTaskAction = {"Find Work Task Action", "Current find work task actions"};
    TConstructWallAction tConstructWallAction = {"Construct wall action", "Current construct wall actions"};
    TConstructDoorAction tConstructDoorAction = {"Construct door action", "Current construct door actions"};

    //organism stuff
    TBloodValues tBloodValues = {"Blood Values", "The content of vital compounds in the blood of an organism"};
    TChoking tChoking = {"Choking", "Lists the impaired breathing capabilities of organisms that are currently not able to absorb oxygen to 100%"};
    IdSet died = {{}, {"Died", "The organisms that have just died, will only be set for one frame"}};
    
    //tasks
    TTask tTask = {"Task", "Represents all tasks"};
    TRoomTask tRoomTask = {"Room Task", "Represents all rooms that need to be built"};
    TWallTask tWallTask = {"Wall Task", "Represents all walls that need to be built"};
    TDoorTask tDoorTask = {"Door Task", "Represents all doors that need to be built"};
    IdSet unassignedTasks = {{}, {"Unassigned tasks", "All tasks which no worker is currently on"}};
    TAssignedTask tAssignedTask = {"Assigned tasks", "All tasks which are assigned to a worker"};
    //items
    TItem tItem = {"Item", "Represents all items"};
    TWearable tWearable = {"Wearable", "Represents items that can be worn"};
    TItemStoring tItemStoring = {"Item Storing", "Represents the fact than an item is stored in a container"};
    //containers
    TItemContainer tItemContainer = {"Item Container", "Everything that can contain items"};
    TAirTank tAirTank = {"Air Tank", "Containers that contain gases"};

    //world
    TDoor tDoor = {"Door", "Represents all doors"};
    TStructureDoorLock tStructureDoorLock = {"Structure Door Lock", "All doors that are locked by structures"};
    IdSet openDoors = {{}, {"Open doors", "All doors that are open"}};
    IdSet lockedDoors = {{}, {"Locked doors", "All doors that are locked"}};
    TZoneLeak tZoneLeak = {"Zone Leak", "Zone leaks are holes between atmosphere zones that transfer air"};

    //structure
    TStructure tStructure = {"Structure", "All existing structures"};
    TStructureType tStructureType = {"Structure Type", "All types of structures"};
    TAirlock tAirlock = {"Airlock", "All existing airlocks"};
    TCrate tCrate = {"Crate", "All existing crates"};
    TAirlockActivity tAirlockActivity = {"Airlock Activity", "Airlocks that are currently pumping air"};
    IdSet uninitializedStructures = {{}, {"Uninitialized Structures", "Structures that need initialization"}};

    //gfx
    TActorSprite tActorSprite = {"Actor Sprite", "Many-to-many relationship between game objects and sprites. Represents the visibility on screen of game entities"};

    //world
    WallMap walls;
    WallChanges wallChanges;
    Zones zones;
    Grid<Gases> atmosphere;
    Grid<GridNeighbors<Gases>> atmosphereNeighbors;
};
