#include "taskutil.hpp"
#include "wallutil.hpp"
#include "data.hpp"

void assignTask(int32_t taskId, int32_t actorId, GameData& data)
{
    insert(taskId, {actorId}, data.tAssignedTask);
    insert(actorId, {taskId}, data.tBusyWorker);
    erase(actorId, data.freeWorkers);
}

std::string toString(Task::Type type)
{
    if(type == Task::Room)
        return "Room";
    else if(type == Task::Door)
        return "Door";
    else if(type == Task::Wall)
        return "Wall";
    else
    {
        TH_ASSERT(false, "unknown task");
        return "bleh";
    }
}

bool hasWallTask(WallPosition position, GameData& data)
{
    return static_cast<bool>(findOne([&] (int32_t id, const WallTask& wallTask)
    {
        return wallTask.position == position;
    }, data.tWallTask));
}

bool hasDoorTask(WallPosition position, GameData& data)
{
    return static_cast<bool>(findOne([&] (int32_t id, const DoorTask& doorTask)
    {
        return doorTask.position == position;
    }, data.tDoorTask));
}

float distanceToTask(const glm::vec2& position, int32_t taskId, GameData& data)
{
    const Task& task = get(taskId, data.tTask);

    if(task.type == Task::Wall)
    {
        const WallTask& wallTask = get(taskId, data.tWallTask);
        return glm::distance(position, wallCenter(wallTask.position));
    }
    else if(task.type == Task::Door)
    {
        const DoorTask& doorTask = get(taskId, data.tDoorTask);
        return glm::distance(position, wallCenter(doorTask.position));
    }
    else
        return 0.0f;
}

int32_t closestTask(const glm::vec2& position, const IdSet& taskIds, GameData& data)
{
    TH_ASSERT(!taskIds.ids.empty(), "Cannot find closest of zero tasks");
    int32_t lowestId = taskIds.ids[0];
    float lowestDistance = distanceToTask(position, lowestId, data);

    for(size_t i = 1; i < taskIds.ids.size(); ++i)
    {
        int32_t thisId = taskIds.ids[i];
        float thisDistance = distanceToTask(position, thisId, data);

        if(thisDistance < lowestDistance)
        {
            lowestDistance = thisDistance;
            lowestId = thisId;
        }

        if(lowestDistance == 0.0f)
            break;
    }

    return lowestId;
}
