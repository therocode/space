#pragma once
#include <data.hpp>
#include "actionutil.hpp"

template <typename TaskTable>
void addTask(typename TaskTable::Type taskData, TaskTable& table, GameData& data)
{
    int32_t newId = insert({100, TaskTable::Type::type}, data.tTask).id;
    insert(newId, std::move(taskData), table);
    insert(newId, data.unassignedTasks);
}

template <typename TaskTable>
void eraseTask(int32_t id, TaskTable& table, GameData& data)
{
    auto taskActionEntry = findOne([&] (int32_t taskActionId, const TaskAction& taskAction)
    {
        return taskAction.taskId == id;
    }, data.tTaskAction);

    if(taskActionEntry)
    {
        clearActions(get(taskActionEntry->data.actionId, data.tAction).actorId, data);
    }
    erase(id, data.tTask);
    erase(id, table);
    erase(id, data.unassignedTasks);
    erase(id, data.tAssignedTask);
}

void assignTask(int32_t taskId, int32_t actorId, GameData& data);

std::string toString(Task::Type type);
