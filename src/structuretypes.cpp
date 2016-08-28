#include "structuretypes.hpp"

void loadStructureTypes(TStructureType& types, const ResourceManager& resources)
{
    insert(Airlock, StructureType
    {
        "Airlock",
        resources.textureFromName("airlock"),
    }, types);
    insert(CryoPods, StructureType
    {
        "Cryo pods",
        resources.textureFromName("cryopods"),
    }, types);
    insert(Battery, StructureType
    {
        "Battery",
        resources.textureFromName("battery"),
    }, types);
    insert(Crate, StructureType
    {
        "Crate",
        resources.textureFromName("crate"),
    }, types);
    insert(Toilet, StructureType
    {
        "Toilet",
        resources.textureFromName("toilet"),
    }, types);
}
