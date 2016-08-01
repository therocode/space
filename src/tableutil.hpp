#pragma once
#include <thero/optional.hpp>
#include "insert.hpp"

template <typename Table>
void tableEmplaceOptional(int32_t id, th::Optional<typename Table::Type> data, Table& table)
{
    if(data)
        insert(id, std::move(*data), table);
}
