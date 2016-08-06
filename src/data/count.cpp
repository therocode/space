#include "count.hpp"

int32_t count(IdSet& idSet)
{
    return static_cast<int32_t>(idSet.ids.size());
}
