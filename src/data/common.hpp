#pragma once
#include <thero/optional.hpp>
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

template <typename DataType>
struct TableMeta
{
    std::string name;
    th::Optional<std::string> description;
    //sort
    bool sorted = true;
    std::vector<size_t> permutationCache;
    std::vector<int32_t> idSortCache;
    std::vector<DataType> dataSortCache;
};

template<typename DataType>
struct DataTable
{
    DataTable(std::string name) { meta.name = name;}
    DataTable(std::string name, std::string description) { meta.name = std::move(name); meta.description = std::move(description); }
    using Type = DataType;
    std::vector<int32_t> ids;
    std::vector<DataType> data;
    TableMeta<DataType> meta;
};

struct IdSetMeta
{
    std::string name;
    th::Optional<std::string> description;
    //sort
    bool sorted = true;
};

struct IdSet
{
    std::vector<int32_t> ids;
    IdSetMeta meta;
};
