#pragma once
#include <cstdint>
#include <cstddef>
#include "data.hpp"

struct MemoryInfo
{
    size_t dataSize;
    size_t totalSize;
};

MemoryInfo memoryInfo(const IdSet& idSet);

template <typename DataTable>
MemoryInfo memoryInfo(const DataTable& table)
{
    MemoryInfo info;

    info.dataSize = sizeof(int32_t) * table.ids.size() + sizeof(typename DataTable::Type) * table.data.size();
    info.totalSize = sizeof(int32_t) * table.ids.capacity() + 
        sizeof(typename DataTable::Type) * table.data.capacity() +
        sizeof(size_t) * table.meta.permutationCache.capacity() +
        sizeof(int32_t) * table.meta.idSortCache.capacity() +
        sizeof(typename DataTable::Type) * table.meta.dataSortCache.capacity() +
        sizeof(DataTable);

    return info;
}
