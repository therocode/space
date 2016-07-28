#pragma once
#include <cstdint>
#include <algorithm>
#include "common.hpp"

bool has(int32_t id, const IdSet& idSet);

template <typename DataType>
bool has(int32_t id, const DataTable<DataType>& table)
{
    return std::find(table.ids.begin(), table.ids.end(), id) != table.ids.end();
}

template <typename... IdContainer>
bool has(int32_t id, const IdContainer&... idContainers)
{
    using swallow = int[];

    bool hasAll = true;

    (void)swallow{((hasAll &= (has(id, idContainers))), 0)...};
    return hasAll;
}

template <typename Functor>
bool has(Functor f, const IdSet& idSet)
{
    return std::find_if(idSet.ids.begin(), idSet.ids.end(), f) != idSet.ids.end();
}

template <typename Functor, typename DataType>
bool has(Functor f, const DataTable<DataType>& table)
{
    auto idIter = table.ids.begin();
    auto dataIter = table.data.begin();

    for(; idIter != table.ids.end(); ++idIter, ++dataIter)
    {
        if(f(*idIter, *dataIter))
            return true;
    }

    return false;
}

template <typename Functor, typename... IdContainer>
bool has(Functor f, const IdContainer&... idContainers)
{
    using swallow = int[];

    bool hasAll = true;

    (void)swallow{((hasAll &= (has(f, idContainers))), 0)...};
    return hasAll;
}
