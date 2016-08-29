#include "structuretypes.hpp"
#include "structures/airlock.hpp"
#include "doorutil.hpp"

void loadStructureTypes(TStructureType& types, const ResourceManager& resources)
{
    insert(Structures::Airlock, StructureType
    {
        "Airlock",
        resources.textureFromName("airlock"),
    }, types);
    insert(Structures::CryoPods, StructureType
    {
        "Cryo pods",
        resources.textureFromName("cryopods"),
    }, types);
    insert(Structures::Battery, StructureType
    {
        "Battery",
        resources.textureFromName("battery"),
    }, types);
    insert(Structures::Crate, StructureType
    {
        "Crate",
        resources.textureFromName("crate"),
    }, types);
    insert(Structures::Toilet, StructureType
    {
        "Toilet",
        resources.textureFromName("toilet"),
    }, types);
}

void createStructure(Structure structure, GameData& data)
{
    int32_t newId = insert(std::move(structure), data.tStructure);
    insert(newId, data.uninitializedStructures);

    int32_t type = structure.structureType;

    if(type == Structures::Airlock)
    {
        insert(newId, Airlock{}, data.tAirlock);
    }
}

void initializeStructure(int32_t id, const Structure& structure, GameData& data)
{
    int32_t type = structure.structureType;

    if(type == Structures::Airlock)
    {
        discoverAirlockDoors(id, structure, data);
    }
}

void updateStructure(int32_t id, const Structure& structure, GameData& data)
{
    int32_t type = structure.structureType;

    if(type == Structures::Airlock)
    {
        const Airlock& airlock = get(id, data.tAirlock);
        for(const auto& door : airlock.doors)
        {
            lockDoor(door, data);
        }
    }
}
