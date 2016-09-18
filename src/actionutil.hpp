#pragma once
#include <data.hpp>

void clearActions(int32_t actorId, GameData& data);

template <typename ActionType, typename DataTable>
int32_t addAction(int32_t aiId, ActionType action, DataTable& table, GameData& data)
{
    int32_t newActionId = insert(Action{aiId, {}, ActionType::type}, data.tAction);
    insert(aiId, std::move(action), table);
    insert(newActionId, data.leafActions);
    return newActionId;
}

void createAction(int32_t aiId, Ai::Type aiType, Incentive::Type incentiveType, GameData& data);
void createAction(int32_t aiId, Ai::Type aiType, int32_t taskId, Task::Type taskType, GameData& data);
template <typename ActionType, typename DataTable>
int32_t addChildAction(int32_t aiId, int32_t parentActionId, ActionType action, DataTable& table, GameData& data)
{
    int32_t newActionId = insert(Action{aiId, {parentActionId}, ActionType::type}, data.tAction);
    insert(aiId, std::move(action), table);
    erase(parentActionId, data.leafActions);
    insert(newActionId, data.leafActions);
    return newActionId;
}
std::string toString(Action::Type type);
