#include "taskutil.hpp"
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
