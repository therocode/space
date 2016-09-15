#include <data.hpp>
#include "resourcemanager.hpp"

namespace Structures
{
    enum {Airlock, CryoPods, Battery, Crate, Toilet};
}

void loadStructureTypes(TStructureType& types, const ResourceManager& resources);
void createStructure(Structure structure, GameData& data);
void initializeStructure(int32_t id, const Structure& structure, GameData& data);
th::Optional<int32_t> structureProvidesPath(int32_t structureId, const WallPosition& wallPosition, const GameData& data);
