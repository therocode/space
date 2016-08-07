#pragma once
#include "common.hpp"
#include <algorithm>
#include <vector>
#include <numeric>

void sort(const IdSet& idSet);

template <typename DataTable>
void sort(const DataTable& cTable)
{
    DataTable& table = const_cast<DataTable&>(cTable);
    if(!table.meta.sorted)
    {
        ++table.meta.metrics[AccessType::Sort];

        std::vector<size_t>& permutations = table.meta.permutationCache;
        permutations.resize(table.ids.size());

        std::iota(permutations.begin(), permutations.end(), 0);
        std::sort(permutations.begin(), permutations.end(), [&] (size_t i, size_t j)
        {
            return table.ids[i] < table.ids[j];
        });

        std::vector<int32_t>& sortedIds = table.meta.idSortCache = table.ids;
        std::vector<typename DataTable::Type>& sortedData = table.meta.dataSortCache = table.data;

        std::transform(permutations.begin(), permutations.end(), sortedIds.begin(), [&] (std::size_t i)
        {
            return table.ids[i];
        });
        std::transform(permutations.begin(), permutations.end(), sortedData.begin(), [&] (std::size_t i)
        {
            return table.data[i];
        });

        table.ids = sortedIds;
        table.data = sortedData;

        table.meta.sorted = true;
    }
}
