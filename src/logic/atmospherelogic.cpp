#include "atmospherelogic.hpp"
#include "../debug.hpp"

AtmosphereLogic::AtmosphereLogic(GameData& data):
    mData(data)
{
}

void AtmosphereLogic::update(const Grid<GridNeighbors<Gases>>& allNeighbors)
{
    if(mAtmosphereDifference.size().x == 0)
        mAtmosphereDifference = {mData.atmosphere.size(), Gases{}};

    mAtmosphereDifference.fill(Gases{});

    std::array<int32_t, 4> neighborDifferences;

    const GridNeighbors<Gases>* startNeighborsPointer = &allNeighbors.at({0, 0});
    const int32_t* startZoneIdPointer = &mData.zones.zones.at({0, 0});
    Gases* startGasesPointer = &mData.atmosphere.at({0, 0});
    Gases* startGasesDifferencePointer = &mAtmosphereDifference.at({0, 0});
    size_t amount = static_cast<size_t>(mData.zones.zones.size().x * mData.zones.zones.size().y);
    for(size_t currentTileIndex = 0; currentTileIndex < amount; ++currentTileIndex)
    {
        const GridNeighbors<Gases>& neighbors = startNeighborsPointer[currentTileIndex];
        Gases gasDifference;
        int32_t zoneId = startZoneIdPointer[currentTileIndex];
        const Gases& currentGases = startGasesPointer[currentTileIndex];
        size_t neighborAmount = neighbors.neighborCount;
        std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);

        for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
        {
            const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

            for(size_t gasIndex = 0; gasIndex < currentGases.size(); ++gasIndex)
            {
                neighborDifferences[neighborIndex] += (currentGases[gasIndex] - neighborGases[gasIndex]);
            }
        }

        int32_t totalDifference = 0;
        for(size_t differenceIndex = 0; differenceIndex < neighborAmount; ++differenceIndex)
            totalDifference += neighborDifferences[differenceIndex];

        float transferRate = std::abs(totalDifference / 10000.0f) / neighborAmount;
        transferRate = std::min(0.0625f, std::max(0.05f, transferRate));

        if(totalDifference)
        {
            auto& sourceGasesDifference = startGasesDifferencePointer[currentTileIndex];
            for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
            {
                auto& targetGasesDifference = startGasesDifferencePointer[neighbors.neighbors[neighborIndex].first];
                const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

                if(neighborDifferences[neighborIndex])
                {
                    for(size_t gasIndex = 0; gasIndex < targetGasesDifference.size(); ++gasIndex)
                    {
                        int32_t toTransfer = static_cast<int32_t>((currentGases[gasIndex] - neighborGases[gasIndex]) * transferRate);
                        if(toTransfer > 0)
                        {
                            targetGasesDifference[gasIndex] += toTransfer;
                            sourceGasesDifference[gasIndex] -= toTransfer;
                        }
                    }
                }
            }
        }
    }

    for(size_t currentTileIndex = 0; currentTileIndex < amount; ++currentTileIndex)
    {
        const Gases& gasDifference = startGasesDifferencePointer[currentTileIndex];
        Gases& gases = startGasesPointer[currentTileIndex];
        for(size_t gasIndex = 0; gasIndex < gasDifference.size(); ++gasIndex)
        {
            gases[gasIndex] += gasDifference[gasIndex];
        }
    }

    //leaks

    forEach([&] (int32_t id, const auto& leak)
    {
        Gases& sourceGases = mData.atmosphere.at(leak.start);
        Gases& targetGases = mData.atmosphere.at(leak.end);

        int64_t difference = pressure(sourceGases) - pressure(targetGases);
        float transferRate = difference / 10000.0f;
        if(transferRate >= 0.0f)
            transferRate = std::min(0.0625f, std::max(0.05f, transferRate));
        else
            transferRate = std::max(-0.0625f, std::min(-0.05f, transferRate));

        for(size_t gasIndex = 0; gasIndex < sourceGases.size(); ++gasIndex)
        {
            int32_t toTransfer = static_cast<int32_t>(std::abs(sourceGases[gasIndex] - targetGases[gasIndex]) * transferRate) + leak.pumpForce;
            int32_t signBit = toTransfer >= 0 ? 1 : -1;
            toTransfer = std::min(std::abs(toTransfer), leak.size) * signBit;
            toTransfer = std::min(toTransfer, sourceGases[gasIndex]);

            if(toTransfer > 0)
            {
                targetGases[gasIndex] += toTransfer;
                sourceGases[gasIndex] -= toTransfer;
            }
        }
    }, mData.tZoneLeak);
}
