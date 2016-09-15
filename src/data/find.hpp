#pragma once
#include "common.hpp"
#include <thero/optional.hpp>

template <typename DataTable>
typename DataTable::Type* findOne(int32_t id, DataTable& table)
{
    ++table.meta.metrics[AccessType::RandomAccess];
    if(!table.meta.sorted)
        sort(table);

    //can be optimised by sorting and binary searching
    for(auto iter = table.ids.begin(); iter != table.ids.end(); ++iter)
    {
        if(*iter == id)
            return &table.data[static_cast<size_t>(std::distance(table.ids.begin(), iter))];
    }

    return nullptr;
}

template <typename DataTable>
typename DataTable::Type* findOne(int32_t id, const DataTable& table)
{
    auto nonConst = findOne(id, const_cast<DataTable&>(table));

    if(nonConst)
        return &nonConst->data;
    else
        return nullptr;
}

template <typename DataTable, typename Functor>
th::Optional<TableEntry<typename DataTable::Type>> findOne(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::RandomAccess];
    if(!table.meta.sorted)
        sort(table);

    for(auto iter = table.data.begin(); iter != table.data.end(); ++iter)
    {
        int32_t id = table.ids[static_cast<size_t>(std::distance(table.data.begin(), iter))];
        if(f(id, *iter))
            return TableEntry<typename DataTable::Type>{id, *iter};
    }

    return {};
}

template <typename DataTable, typename Functor>
th::Optional<TableEntry<typename DataTable::Type>> findOne(Functor f, const DataTable& table)
{
    auto nonConst = findOne(f, const_cast<DataTable&>(table));

    if(nonConst)
        return nonConst;
    else
        return {};
}
