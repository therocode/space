#include "structuretypes.hpp"
#include "structures/airlock.hpp"
#include "doorutil.hpp"

void loadStructureTypes(TStructureType& types, const ResourceManager& resources)
{
    insert(Structure::Airlock, StructureType
    {
        "Airlock",
        resources.textureFromName("airlock"),
    }, types);
    insert(Structure::CryoPods, StructureType
    {
        "Cryo pods",
        resources.textureFromName("cryopods"),
    }, types);
    insert(Structure::Battery, StructureType
    {
        "Battery",
        resources.textureFromName("battery"),
    }, types);
    insert(Structure::Crate, StructureType
    {
        "Crate",
        resources.textureFromName("crate"),
    }, types);
    insert(Structure::Toilet, StructureType
    {
        "Toilet",
        resources.textureFromName("toilet"),
    }, types);
}

int32_t createStructure(Structure structure, GameData& data)
{
    int32_t newId = insert(std::move(structure), data.tStructure).id;
    insert(newId, data.uninitializedStructures);

    int32_t type = structure.type;

    if(type == Structure::Airlock)
    {
        insert(newId, Airlock{Airlock::In, {}, {}}, data.tAirlock);
    }
    else if(type == Structure::Crate)
    {
        const auto& containerEntry = insert({}, data.tItemContainer);
        insert(newId, Crate{containerEntry.id}, data.tCrate);
    }

    return newId;
}

void initializeStructure(int32_t id, const Structure& structure, GameData& data)
{
    int32_t type = structure.type;

    if(type == Structure::Airlock)
    {
        discoverAirlockDoors(id, structure, data);
    }
}

th::Optional<int32_t> structureProvidesPath(int32_t structureId, const WallPosition& wallPosition, const GameData& data)
{
    Structure structure = get(structureId, data.tStructure);
    int32_t type = structure.type;

    if(type == Structure::Airlock)
    {
        return airlockProvidesPath(structureId, wallPosition, data);
    }

    return {};
}
