#include "itemutil.hpp"
#include "itemtypes.hpp"

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
