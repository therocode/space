#pragma once
#include "common.hpp"

void clear(IdSet& idSet);

template <typename DataTable>
void clear(DataTable& table)
{
    table.ids.clear();
    table.data.clear();
    table.meta.sorted = true;
}
