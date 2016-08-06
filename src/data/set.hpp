#pragma once
#include "common.hpp"

template <typename DataTable>
void set(int32_t id, typename DataTable::Type value, DataTable& table)
{
    auto entry = findOne(id, table);

    if(entry)
    {
        entry->data = std::move(value);
    }
    else
    {
        insert(id, std::move(value), table);
    }
}
