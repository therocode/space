#include "sort.hpp"

void sort(const IdSet& cIdSet)
{
    IdSet& idSet = const_cast<IdSet&>(cIdSet);
    if(!idSet.meta.sorted)
    {
        ++idSet.meta.metrics[AccessType::Sort];
        std::sort(idSet.ids.begin(), idSet.ids.end());
        idSet.meta.sorted = true;
    }
}
