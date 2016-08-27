#include "structuretypes.hpp"

void loadStructureTypes(TStructureType& types, const ResourceManager& resources)
{
    insert(StructureType
    {
        "Airlock",
        resources.textureFromName("airlock"),
    }, types);
    insert(StructureType
    {
        "Cryo pods",
        resources.textureFromName("cryopods"),
    }, types);
    insert(StructureType
    {
        "Battery",
        resources.textureFromName("battery"),
    }, types);
    insert(StructureType
    {
        "Crate",
        resources.textureFromName("crate"),
    }, types);
}
