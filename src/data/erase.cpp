#include "erase.hpp"

void erase(int32_t id, IdSet& idSet)
{
    idSet.ids.erase(std::remove(idSet.ids.begin(), idSet.ids.end(), id), idSet.ids.end());
}
