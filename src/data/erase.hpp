#pragma once
#include <algorithm>
#include <cstdint>
#include "common.hpp"

void erase(int32_t id, IdSet& idSet);

template <typename DataTable>
void erase(int32_t id, DataTable& table)
{
    ++table.meta.metrics[AccessType::Deletion];
    auto toErase = std::find(table.ids.begin(), table.ids.end(), id);

    if(toErase != table.ids.end())
    {
        table.data.erase(table.data.begin() + std::distance(table.ids.begin(), toErase));
        table.ids.erase(toErase);
    }

    table.meta.sorted = false;
}

template <typename Functor>
void eraseIf(Functor f, IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::Iteration];

    size_t beforeSize = idSet.ids.size();
    for(auto iter = idSet.ids.begin(); iter != idSet.ids.end();)
    {
        if(f(*iter))
        {
            ++idSet.meta.metrics[AccessType::Deletion];
            iter = idSet.ids.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    if(beforeSize != idSet.ids.size())
        idSet.meta.sorted = false;
}

template <typename Functor, typename DataTable>
void eraseIf(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    size_t beforeSize = table.ids.size();
    for(; idIter != table.ids.end();)
    {
        if(f(*idIter, *dataIter))
        {
            ++table.meta.metrics[AccessType::Deletion];
            idIter = table.ids.erase(idIter);
            dataIter = table.data.erase(dataIter);
        }
        else
        {
            ++idIter;
            ++dataIter;
        }
    }

    if(beforeSize != table.ids.size())
        table.meta.sorted = false;
}
