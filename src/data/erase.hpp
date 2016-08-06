#pragma once
#include <algorithm>
#include <cstdint>
#include "common.hpp"

void erase(int32_t id, IdSet& idSet);

template <typename DataTable>
void erase(int32_t id, DataTable& table)
{
    auto toErase = std::find(table.ids.begin(), table.ids.end(), id);

    if(toErase != table.ids.end())
    {
        table.data.erase(table.data.begin() + std::distance(table.ids.begin(), toErase));
        table.ids.erase(toErase);
    }
}

template <typename Functor>
void eraseIf(Functor f, IdSet& idSet)
{
    for(auto iter = idSet.ids.begin(); iter != idSet.ids.end();)
    {
        if(f(*iter))
        {
            iter = idSet.ids.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

template <typename Functor, typename DataTable>
void eraseIf(Functor f, DataTable& table)
{
    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    for(; idIter != table.ids.end();)
    {
        if(f(*idIter, *dataIter))
        {
            idIter = table.ids.erase(idIter);
            dataIter = table.data.erase(dataIter);
        }
        else
        {
            ++idIter;
            ++dataIter;
        }
    }
}

