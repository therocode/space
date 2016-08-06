#include "insert.hpp"

void insert(int32_t id, IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::Addition];
    idSet.ids.push_back(id);
}
