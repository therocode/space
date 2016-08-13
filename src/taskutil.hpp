#pragma once
#include "data.hpp"

template <typename TaskTable>
void addTask(int32_t id, typename TaskTable::Type data, TaskTable& table, IdSet& unassignedTasks)
{
    insert(id, std::move(data), table);
    insert(id, unassignedTasks);
}
