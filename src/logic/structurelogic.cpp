#include "structurelogic.hpp"
#include "../structuretypes.hpp"

StructureLogic::StructureLogic(GameData& data, const ResourceManager& resources):
    mData(data)
{
    loadStructureTypes(mData.tStructureType, resources);
}

void StructureLogic::update()
{
}
