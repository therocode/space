#include "taskutil.hpp"
#include "data.hpp"

void assignTask(int32_t taskId, int32_t actorId, TAssignedTask& tAssignedTask)
{
    insert(taskId, {actorId}, tAssignedTask);
}
