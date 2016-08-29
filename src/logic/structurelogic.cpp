#include "structurelogic.hpp"
#include "../structuretypes.hpp"

StructureLogic::StructureLogic(GameData& data, const ResourceManager& resources):
    mData(data)
{
    loadStructureTypes(mData.tStructureType, resources);
}

void StructureLogic::update()
{
    forEach([&] (int32_t id, Structure& structure)
    {
        updateStructure(id, structure, mData);
    }, mData.tStructure);
}

void StructureLogic::updateAfterWall()
{
    eraseIf([&] (int32_t id)
    {
        const Structure& structure = get(id, mData.tStructure);
        initializeStructure(id, structure, mData);       
        return true;
    }, mData.uninitializedStructures);
}
