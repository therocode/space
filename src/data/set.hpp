#pragma once
#include "common.hpp"

template <typename DataType>
void set(int32_t id, DataType value, DataTable<DataType, true>& table)
{
    auto entry = findOne(id, table);

    if(entry)
    {
        *entry = std::move(value);
    }
    else
    {
        insert(id, std::move(value), table);
    }
}

template <typename DataType>
void set(int32_t id, DataType value, DataTable<DataType, false>& table)
{
    auto entry = findOne(id, table);

    if(entry)
    {
        *entry = std::move(value);
    }
    TH_ASSERT(false, "Cannot set value of id '" << id << "' in a table that doesn't have that ID and provides own IDs");
}
