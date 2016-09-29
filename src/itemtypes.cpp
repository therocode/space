#include "itemtypes.hpp"

int32_t createItem(Item item, GameData& data)
{
    int32_t newId = insert(std::move(item), data.tItem).id;

    int32_t type = item.type;

    if(type == Item::SpaceSuit)
    {
        insert(newId, {}, data.tWearable);
    }

    return newId;
}
