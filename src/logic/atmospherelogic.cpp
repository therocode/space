#include "atmospherelogic.hpp"
#include "../debug.hpp"

AtmosphereLogic::AtmosphereLogic(const Zones& zones, const WallMap& walls, Grid<Gases>& atmosphere):
    mZones(zones),
    mWalls(walls),
    mAtmosphere(atmosphere),
    mAtmosphereDifference(mAtmosphere.size(), Gases{})
{
    (void)mWalls;
}

void AtmosphereLogic::update(const Grid<GridNeighbors<Gases>>& allNeighbors)
{
    mAtmosphereDifference.fill(Gases{});

    std::array<int32_t, 4> neighborDifferences;

    const GridNeighbors<Gases>* startNeighborsPointer = &allNeighbors.at({0, 0});
    const int32_t* startZoneIdPointer = &mZones.zones.at({0, 0});
    Gases* startGasesPointer = &mAtmosphere.at({0, 0});
    Gases* startGasesDifferencePointer = &mAtmosphereDifference.at({0, 0});
    size_t amount = static_cast<size_t>(mZones.zones.size().x * mZones.zones.size().y);
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
}
