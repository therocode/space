#pragma once
#include <thero/optional.hpp>
#include "insert.hpp"

template <typename Table>
void tableEmplaceOptional(int32_t id, th::Optional<typename std::decay<decltype(Table().data[0])>::type> data, Table& table)
{
    if(data)
        insert(id, std::move(*data), table);
}
