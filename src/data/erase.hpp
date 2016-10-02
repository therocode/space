#pragma once
#include <algorithm>
#include <cstdint>
#include "common.hpp"

void erase(int32_t id, IdSet& idSet);

template <typename DataType>
void erase(int32_t id, DataTable<DataType, true>& table)
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

template <typename DataType>
void erase(int32_t id, DataTable<DataType, false>& table)
{
    ++table.meta.metrics[AccessType::Deletion];
    auto toErase = std::find(table.ids.begin(), table.ids.end(), id);

    if(toErase != table.ids.end())
    {
        table.data.erase(table.data.begin() + std::distance(table.ids.begin(), toErase));
        table.ids.erase(toErase);
        table.meta.idPool.release(id);
    }

    table.meta.sorted = false;
}

template <typename Functor>
int32_t eraseIf(Functor f, IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::Iteration];

    size_t beforeSize = idSet.ids.size();
    int32_t erasedAmount = 0;
    for(auto iter = idSet.ids.begin(); iter != idSet.ids.end();)
    {
        if(f(*iter))
        {
            ++idSet.meta.metrics[AccessType::Deletion];
            iter = idSet.ids.erase(iter);
            ++erasedAmount;
        }
        else
        {
            ++iter;
        }
    }

    if(beforeSize != idSet.ids.size())
        idSet.meta.sorted = false;

    return erasedAmount;
}

template <typename Functor, typename DataType>
int32_t eraseIf(Functor f, DataTable<DataType, true>& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    size_t beforeSize = table.ids.size();
    int32_t erasedAmount = 0;
    for(; idIter != table.ids.end();)
    {
        if(f(*idIter, *dataIter))
        {
            ++table.meta.metrics[AccessType::Deletion];
            idIter = table.ids.erase(idIter);
            dataIter = table.data.erase(dataIter);
            ++erasedAmount;
        }
        else
        {
            ++idIter;
            ++dataIter;
        }
    }

    if(beforeSize != table.ids.size())
        table.meta.sorted = false;

    return erasedAmount;
}

template <typename Functor, typename DataType>
int32_t eraseIf(Functor f, DataTable<DataType, false>& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    size_t beforeSize = table.ids.size();
    int32_t erasedAmount = 0;
    for(; idIter != table.ids.end();)
    {
        if(f(*idIter, *dataIter))
        {
            ++table.meta.metrics[AccessType::Deletion];
            table.meta.idPool.release(*idIter);
            idIter = table.ids.erase(idIter);
            dataIter = table.data.erase(dataIter);
            ++erasedAmount;
        }
        else
        {
            ++idIter;
            ++dataIter;
        }
    }

    if(beforeSize != table.ids.size())
        table.meta.sorted = false;

    return erasedAmount;
}
