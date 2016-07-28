#include "has.hpp"

bool has(int32_t id, const IdSet& idSet)
{
    return std::find(idSet.ids.begin(), idSet.ids.end(), id) != idSet.ids.end();
}
