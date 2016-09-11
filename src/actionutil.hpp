#pragma once
#include <data.hpp>

void clearActions(int32_t actorId, GameData& data);

template <typename ActionType, typename DataTable>
void addAction(int32_t aiId, ActionType action, DataTable& table, GameData& data)
{
    int32_t newActionId = insert(Action{aiId, {}, ActionType::type}, data.tAction);
    insert(aiId, std::move(action), table);
}

void createAction(int32_t aiId, Ai::Type aiType, Incentive::Type incentiveType, GameData& data);
