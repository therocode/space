#include "organismlogic.hpp"

OrganismLogic::OrganismLogic(EntityData& ent, Grid<Gases>& atmosphere):
    mEnt(ent),
    mAtmosphere(atmosphere)
{
}

void OrganismLogic::update()
{
    clear(mEnt.died);
    clear(mEnt.tChoking);

    join([&] (int32_t id, const glm::vec2& position, BloodValues& bloodValue)
    {
        glm::ivec2 tile = position / 32.0f;       

        Gases& gases = mAtmosphere.at(tile);
        int64_t totalGasAmount = pressure(gases);
        float pressure = pressurePercent(gases);
        float oxygenPercent = gases[Oxygen] / static_cast<float>(totalGasAmount);

        float humanNormalizedOxygen = (oxygenPercent - 0.17f) / (0.21f - 0.17f);
        float breathingCapability = humanNormalizedOxygen * pressure;

        bool choking = breathingCapability < 0.9f;

        if(choking)
        {
            insert(id, {breathingCapability}, mEnt.tChoking);
        }

        if(bloodValue.oxygen < 600)
        {
		    int32_t amount = std::max(0, std::min(static_cast<int32_t>(choking ? breathingCapability * 5 : 5), gases[Oxygen]));
		    bloodValue.oxygen += amount;
		    gases[Oxygen] -= amount;
        }

        if(bloodValue.oxygen > 0)
        {
            --bloodValue.oxygen;
		    ++gases[CarbonDioxide];
        }
        else
        {
            insert(id, mEnt.died);
        }

    }, mEnt.tPosition, mEnt.tBloodValues);
}
