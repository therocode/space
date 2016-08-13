#include "taskutil.hpp"
#include "data.hpp"

void assignTask(int32_t taskId, int32_t actorId, tsk::TAssignedTask& tAssignedTask)
{
    insert(taskId, {actorId}, tAssignedTask);
}
