#pragma once
#include <thero/optional.hpp>
#include "insert.hpp"

template <typename DataType>
th::Optional<int32_t> tableEmplaceOptional(int32_t id, th::Optional<DataType> data, DataTable<DataType, true>& table)
{
    if(data)
        return insert(id, std::move(*data), table).id;
    else
        return {};
}

template <typename DataType>
th::Optional<int32_t> tableEmplaceOptional(th::Optional<DataType> data, DataTable<DataType, false>& table)
{
    if(data)
        return insert(std::move(*data), table).id;
    else
        return {};
}
