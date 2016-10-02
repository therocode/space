#include "structureutil.hpp"
#include "doorutil.hpp"
#include "tileutil.hpp"

bool structureLockDoor(int32_t structureId, int32_t doorId, GameData& data)
{
    if(lockDoor(doorId, data))
    {
        insert(StructureDoorLock{doorId, structureId}, data.tStructureDoorLock);
        return true;
    }
    return false;
}

glm::vec2 containerPosition(int32_t containerId, GameData& data)
{
    auto crateEntry = findOne([&] (int32_t crateId, const Crate& crate)
    {
        return crate.containerId == containerId;
    }, data.tCrate);

    if(crateEntry)
    {
        return tileCenter(get(crateEntry->id, data.tStructure).position);
    }

    TH_ASSERT(false, "wtf container has no position");
    return {};
}
