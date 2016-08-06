#include "sort.hpp"

void sort(IdSet& idSet)
{
    std::sort(idSet.ids.begin(), idSet.ids.end());
    idSet.meta.sorted = true;
}
