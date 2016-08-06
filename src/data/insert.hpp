#pragma once
#include "common.hpp"

void insert(int32_t id, IdSet& idSet);

template <typename DataTable>
void insert(int32_t id, typename DataTable::Type data, DataTable& table)
{
    ++table.meta.metrics[AccessType::Addition];
    table.ids.push_back(id);
    table.data.emplace_back(std::move(data));
}
