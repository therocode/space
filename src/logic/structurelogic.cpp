#include "structurelogic.hpp"
#include "../structuretypes.hpp"

StructureLogic::StructureLogic(StructureData& str, const ResourceManager& resources):
    mStr(str)
{
    loadStructureTypes(mStr.tStructureType, resources);
}

void StructureLogic::update()
{
}
