#pragma once
#include <data.hpp>
#include <thero/optional.hpp>
#include <thero/any.hpp>

struct ActionVariant
{
    int32_t actorId;
    th::Optional<int32_t> parentActionId;
    Action::Type type;
    th::Any actionData;
};

struct ActionResult
{
    enum Status { InProgress, Success, Fail };
    Status status = InProgress;
    th::Optional<ActionVariant> createdSubAction = {};
};

struct ActionCreateData
{
    int32_t actorId;
    AiType aiType;
    int32_t taskId;
    Task::Type taskType;
};

void clearActions(int32_t actorId, GameData& data);

template <typename ActionType, typename DataTable>
int32_t addAction(int32_t aiId, ActionType action, DataTable& table, GameData& data)
{
    int32_t newActionId = insert(Action{ActionType::type, aiId, {}}, data.tAction).id;
    insert(newActionId, std::move(action), table);
    insert(newActionId, data.leafActions);
    return newActionId;
}

void createAction(int32_t aiId, AiType aiType, Incentive::Type incentiveType, GameData& data);
void createAction(int32_t aiId, AiType aiType, int32_t taskId, Task::Type taskType, GameData& data);
template <typename ActionType, typename DataTable>
int32_t addChildAction(int32_t aiId, int32_t parentActionId, ActionType action, DataTable& table, GameData& data)
{
    int32_t newActionId = insert(Action{ActionType::type, aiId, {parentActionId}}, data.tAction).id;
    insert(newActionId, std::move(action), table);
    erase(parentActionId, data.leafActions);
    insert(newActionId, data.leafActions);
    return newActionId;
}
std::string toString(Action::Type type);
void deleteAction(int32_t actionId, GameData& data);
void deleteLeafAction(int32_t leafActionId, GameData& data);
