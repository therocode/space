#include "organismlogic.hpp"

OrganismLogic::OrganismLogic(const ent::TPosition& tPosition, ent::TBloodValues& tBloodValues, Grid<Gases>& atmosphere):
    mTPosition(tPosition),
    mTBloodValues(tBloodValues),
    mAtmosphere(atmosphere)
{
}

void OrganismLogic::update()
{
    join([&] (int32_t id, const glm::vec2& position, BloodValues& bloodValue)
    {
        glm::ivec2 tile = position / 32.0f;       

        Gases& gases = mAtmosphere.at(tile);

        if(bloodValue.oxygen < 600)
        {
		    int32_t amount = std::max(0, std::min(5, gases[Oxygen]));
		    bloodValue.oxygen += amount;
		    gases[Oxygen] -= amount;
        }

        if(bloodValue.oxygen > 0)
        {
            --bloodValue.oxygen;
        }
        else
        {
            bloodValue.dead = true;
        }

    }, mTPosition, mTBloodValues);
}
