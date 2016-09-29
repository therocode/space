#pragma once
#include <thero/optional.hpp>
#include <type_traits>
#include <cstdint>
#include <vector>
#include "metrics.hpp"
#include "../util/numberpool.hpp"

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
    NumberPool<int32_t, false> idPool;
    //sort
    mutable bool sorted = true;
    mutable std::vector<size_t> permutationCache;
    mutable std::vector<int32_t> idSortCache;
    mutable std::vector<DataType> dataSortCache;
    //metrics
    mutable Metrics metrics = {};
};

template<typename DataType, bool externalId>
struct DataTable
{
    DataTable(std::string name) { meta.name = name;}
    DataTable(std::string name, std::string description) { meta.name = std::move(name); meta.description = std::move(description); }
    using Type = DataType;
    using ExternalId = std::integral_constant<bool, externalId>;
    std::vector<int32_t> ids;
    std::vector<DataType> data;
    TableMeta<DataType> meta;
};

struct IdSetMeta
{
    std::string name;
    th::Optional<std::string> description;
    //sort
    mutable bool sorted = true;
    //metrics
    mutable Metrics metrics = {};
};

struct IdSet
{
    std::vector<int32_t> ids;
    IdSetMeta meta;
};
