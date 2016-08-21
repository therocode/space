#pragma once
#include "common.hpp"

void clear(IdSet& idSet);

template <typename DataType>
void clear(DataTable<DataType, true>& table)
{
    table.ids.clear();
    table.data.clear();
    table.meta.sorted = true;
}

template <typename DataType>
void clear(DataTable<DataType, false>& table)
{
    for(int32_t id : table.ids)
        table.meta.idPool.release(id);

    table.ids.clear();
    table.data.clear();
    table.meta.sorted = true;
}
