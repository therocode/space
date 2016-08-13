#include "memory.hpp"

MemoryInfo memoryInfo(const IdSet& idSet)
{
    MemoryInfo info;

    info.dataSize = sizeof(int32_t) * idSet.ids.size();
    info.totalSize = sizeof(int32_t) * idSet.ids.capacity() + 
        sizeof(IdSet);

    return info;
}
