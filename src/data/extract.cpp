#include "extract.hpp"

int32_t extractOne(IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::RandomAccess];
    ++idSet.meta.metrics[AccessType::Deletion];
    int32_t extracted = idSet.ids.back();
    idSet.ids.pop_back();
    return extracted;
}
