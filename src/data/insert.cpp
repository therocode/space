#include "insert.hpp"
#include "has.hpp"

int32_t insert(int32_t id, IdSet& idSet)
{
    if(!has(id, idSet))
    {
        ++idSet.meta.metrics[AccessType::Addition];
        idSet.ids.push_back(id);

        if(idSet.ids.size() > 1 && idSet.ids[idSet.ids.size() - 2] > idSet.ids.back())
            idSet.meta.sorted = false;
    }

    return id;
}
