#pragma once
#include "common.hpp"

int32_t count(IdSet& idSet);

template <typename Functor>
int32_t countIf(Functor f, IdSet& idSet)
{
    int32_t amount;

    for(auto iter = idSet.ids.begin(); iter != idSet.ids.end(); ++iter)
    {
        if(f(*iter))
        {
            ++amount;
        }
    }

    return amount;
}

template <typename DataTable>
int32_t count(DataTable& table)
{
    return static_cast<int32_t>(table.ids.size());
}

template <typename Functor, typename DataTable>
int32_t countIf(Functor f, DataTable& table)
{
    int32_t amount = 0;

    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    for(; idIter != table.ids.end(); ++idIter, ++dataIter)
    {
        if(f(*idIter, *dataIter))
        {
            ++amount;
        }
    }

    return amount;
}
