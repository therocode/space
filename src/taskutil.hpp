#pragma once
#include "data.hpp"

template <typename TaskTable>
void addTask(int32_t id, typename TaskTable::Type data, TaskTable& table, IdSet& unassignedTasks)
{
    insert(id, std::move(data), table);
    insert(id, unassignedTasks);
}

template <typename TaskTable>
void eraseTask(int32_t id, TaskTable& table, IdSet& unassignedTasks, tsk::TAssignedTask& tAssignedTask)
{
    erase(id, table);
    erase(id, unassignedTasks);
    erase(id, tAssignedTask);
}

void assignTask(int32_t taskId, int32_t actorId, tsk::TAssignedTask& tAssignedTask);
