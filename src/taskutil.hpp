#pragma once
#include <data.hpp>

template <typename TaskTable>
void addTask(typename TaskTable::Type taskData, TaskTable& table, GameData& data)
{
    int32_t newId = insert({100, TaskTable::Type::type}, data.tTask);
    insert(newId, std::move(taskData), table);
    insert(newId, data.unassignedTasks);
}

template <typename TaskTable>
void eraseTask(int32_t id, TaskTable& table, GameData& data)
{
    erase(id, data.tTask);
    erase(id, table);
    erase(id, data.unassignedTasks);
    erase(id, data.tAssignedTask);
}

void assignTask(int32_t taskId, int32_t actorId, GameData& data);

std::string toString(Task::Type type);
