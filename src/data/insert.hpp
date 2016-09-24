#pragma once
#include "common.hpp"

int32_t insert(int32_t id, IdSet& idSet);

template <typename DataTable>
TableEntry<typename DataTable::Type> insert(int32_t id, typename DataTable::Type data, DataTable& table)
{
    ++table.meta.metrics[AccessType::Addition];
    table.ids.push_back(id);
    table.data.emplace_back(std::move(data));
    typename DataTable::Type& entry = table.data.back();

    if(table.ids.size() > 1 && table.ids[table.ids.size() - 2] > table.ids.back())
        table.meta.sorted = false;

    return {id, entry};
}

template <typename DataTable>
TableEntry<typename DataTable::Type> insert(typename DataTable::Type data, DataTable& table)
{
    int32_t id = table.meta.idPool.next();
    ++table.meta.metrics[AccessType::Addition];
    table.ids.push_back(id);
    table.data.emplace_back(std::move(data));
    typename DataTable::Type& entry = table.data.back();

    if(table.ids.size() > 1 && table.ids[table.ids.size() - 2] > table.ids.back())
        table.meta.sorted = false;

    return {id, entry};
}
