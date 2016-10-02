#include "itemutil.hpp"
#include "itemtypes.hpp"
#include "atmosphereutil.hpp"

int32_t createItemInContainer(Item item, int32_t containerId, GameData& data)
{
    int32_t createdId = createItem(std::move(item), data);

    TH_ASSERT(has(containerId, data.tItemContainer), "container ID " << containerId << " doesn't exist!");

    insert(ItemStoring{createdId, containerId}, data.tItemStoring);

    return createdId;
}

std::string toString(Item::Type type)
{
    if(type == Item::SpaceSuit)
        return "Space Suit";
    else
    {
        TH_ASSERT(false, "unknown item");
        return "bleh";
    }
}

bool needsSpaceSuit(const Path& path, GameData& data)
{
    bool needsSuit = false;
    for(const glm::ivec2& tile : path.path)
    {
        const Gases& gases = data.atmosphere.at(tile);

        needsSuit = !healthyAtmosphere(gases);
        if(needsSuit)
            break;

        forEach([&] (int32_t id, const Structure& structure)
         {
            if(structure.position == tile && has(id, data.tAirlock))
                needsSuit = true;

         }, data.tStructure);
        if(needsSuit)
            break;
    }

    return needsSuit;
}

bool hasSpaceSuit(int32_t actorId, GameData& data)
{
    bool hasSuit = false;

    forEach([&] (int32_t itemId, const Wearable& wearable)
    {
        if(has(itemId, data.tAirTank))
        {
            if(wearable.wearer && *wearable.wearer == actorId)
            {
                hasSuit = true;
            }
        }
    }, data.tWearable);

    return hasSuit;
}
