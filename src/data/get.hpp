#pragma once
#include "common.hpp"
#include <thero/optional.hpp>

template <typename DataTable>
auto& get(int32_t id, DataTable& table)
{
    ++table.meta.metrics[AccessType::RandomAccess];
    if(!table.meta.sorted)
        sort(table);

    auto iter = table.ids.begin();
    for(; iter != table.ids.end(); ++iter)
    {
        if(*iter == id)
            break;
    }

    TH_ASSERT(iter != table.ids.end(), "tried to get id " << id << " from a table which didn't have it");
    return table.data[static_cast<size_t>(std::distance(table.ids.begin(), iter))];
}

template <typename DataTable>
const auto& get(int32_t id, const DataTable& table)
{
    auto& nonConst = get(id, const_cast<DataTable&>(table));
    return nonConst;
}
