#pragma once
#include <type_traits>
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
std::enable_if_t<std::is_same<std::result_of_t<Functor(int32_t, typename DataTable::Type&)>, void>::value> forEach(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    for(size_t i = 0; i < table.ids.size(); ++i)
    {
        f(table.ids[i], table.data[i]);
    }
}

template <typename Functor, typename DataTable>
std::enable_if_t<std::is_same<std::result_of_t<Functor(int32_t, typename DataTable::Type&)>, LoopResult>::value> forEach(Functor f, DataTable& table)
{
    ++table.meta.metrics[AccessType::Iteration];
    for(size_t i = 0; i < table.ids.size(); ++i)
    {
        if(f(table.ids[i], table.data[i]) == LoopResult::Continue)
            continue;
        else
            break;
    }
}
