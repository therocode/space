#include "erase.hpp"

void erase(int32_t id, IdSet& idSet)
{
    ++idSet.meta.metrics[AccessType::Deletion];
    idSet.ids.erase(std::remove(idSet.ids.begin(), idSet.ids.end(), id), idSet.ids.end());
    idSet.meta.sorted = false;
}
