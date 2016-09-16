#include "structureutil.hpp"
#include "doorutil.hpp"

bool structureLockDoor(int32_t structureId, int32_t doorId, GameData& data)
{
    if(lockDoor(doorId, data))
    {
        insert(StructureDoorLock{doorId, structureId}, data.tStructureDoorLock);
        return true;
    }
    return false;
}
