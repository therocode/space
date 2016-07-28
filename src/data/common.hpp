#pragma once
#include <type_traits>
#include <cstdint>
#include <vector>

template<typename DataType>
struct TableEntry
{
    using Type = DataType;
    int32_t id;
    typename std::add_lvalue_reference<DataType>::type data;
};

template<typename DataType>
struct DataTable
{
    using Type = DataType;
    std::vector<int32_t> ids;
    std::vector<DataType> data;
};

struct IdSet
{
    std::vector<int32_t> ids;
};
