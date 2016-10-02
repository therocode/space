#pragma once
#include "common.hpp"

enum LoopResult { Continue, Break };

template <typename Functor>
void forEach(Functor f, IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::Iteration];
    for(size_t i = 0; i < idSet.ids.size(); ++i)
    {
        f(idSet.ids[i]);
    }
}

template <typename Functor, typename DataTable>
void forEach(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    for(size_t i = 0; i < table.ids.size(); ++i)
    {
        f(table.ids[i], table.data[i]);
    }
}
