#include "itemtypes.hpp"

int32_t createItem(Item item, GameData& data)
{
    int32_t newId = insert(std::move(item), data.tItem).id;

    int32_t type = item.type;

    if(type == Item::SpaceSuit)
    {
        int32_t airTank = insert(newId, 
        {
            5000000,
            {{
                static_cast<int32_t>(5000000 * 0.209f), //oxygene
                static_cast<int32_t>(5000000 * 0.78f), //nitrogen
                static_cast<int32_t>(5000000 * 0.004f), //carbondioxide
            }},
        }, data.tAirTank).id;
        insert(newId, {{}, {airTank}}, data.tWearable);
    }

    return newId;
}
