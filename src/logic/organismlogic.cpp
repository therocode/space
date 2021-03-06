#include "organismlogic.hpp"
#include "../atmosphereutil.hpp"

OrganismLogic::OrganismLogic(GameData& data):
    mData(data)
{
}

void OrganismLogic::update()
{
    clear(mData.died);
    clear(mData.tChoking);

    join([&] (int32_t id, const Position& positionStruct, BloodValues& bloodValue)
    {
        const glm::vec2& position = positionStruct.position;
        glm::ivec2 tile = position / 32.0f;       

        th::Optional<int32_t> airTank;
        auto entry = findOne([&](int32_t wearableId, const Wearable& wearable)
        {
            return wearable.airTank && wearable.wearer && *wearable.wearer == id;
        }, mData.tWearable);

        if(entry)
            airTank = *entry->data.airTank;

        Gases& gases = airTank ? get(*airTank, mData.tAirTank).gases : mData.atmosphere.at(tile);
        int64_t totalGasAmount = pressure(gases);
        float pressure = pressurePercent(gases);
        float oxygenPercent = gases[Oxygen] / static_cast<float>(totalGasAmount);

        float humanNormalizedOxygen = (oxygenPercent - 0.17f) / (0.21f - 0.17f);
        float breathingCapability = humanNormalizedOxygen * pressure;

        bool choking = breathingCapability < 0.9f;

        if(choking)
        {
            insert(id, {breathingCapability}, mData.tChoking);
        }

        if(bloodValue.oxygen < 600)
        {
		    int32_t amount = std::max(0, std::min(static_cast<int32_t>(choking ? breathingCapability * 5 : 5), gases[Oxygen]));
		    bloodValue.oxygen += amount;
		    gases[Oxygen] -= amount;

            auto breatheIncentive = findOne([&] (int32_t incentiveId, const BreatheIncentive& incentive)
            {
                return get(incentiveId, mData.tIncentive).actorId == id;
            }, mData.tBreatheIncentive);

            if(breatheIncentive)
            {
                if(bloodValue.oxygen < 450)
                {
                    get(breatheIncentive->id, mData.tIncentive).importance = 550-bloodValue.oxygen;
                }
                else
                {
                    get(breatheIncentive->id, mData.tIncentive).importance = 0;
                }
            }
        }

        if(bloodValue.oxygen > 1)
        {
            bloodValue.oxygen -= 2;
		    ++gases[CarbonDioxide];
        }
        else
        {
            insert(id, mData.died);
        }

    }, mData.tPosition, mData.tBloodValues);
}
