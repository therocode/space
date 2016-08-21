#pragma once
#include "common.hpp"

int32_t insert(int32_t id, IdSet& idSet);

template <typename DataType>
int32_t insert(int32_t id, DataType data, DataTable<DataType, true>& table)
{
    ++table.meta.metrics[AccessType::Addition];
    table.ids.push_back(id);
    table.data.emplace_back(std::move(data));

    if(table.ids.size() > 1 && table.ids[table.ids.size() - 2] > table.ids.back())
        table.meta.sorted = false;

    return id;
}

template <typename DataType>
int32_t insert(DataType data, DataTable<DataType, false>& table)
{
    int32_t id = table.meta.idPool.next();
    ++table.meta.metrics[AccessType::Addition];
    table.ids.push_back(id);
    table.data.emplace_back(std::move(data));

    if(table.ids.size() > 1 && table.ids[table.ids.size() - 2] > table.ids.back())
        table.meta.sorted = false;

    return id;
}
