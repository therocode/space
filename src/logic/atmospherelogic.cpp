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

    size_t currentTileIndex = 0;
    for(int32_t y = 0; y < mZones.zones.size().y; ++y)
    {
        for(int32_t x = 0; x < mZones.zones.size().x; ++x)
        {
            const GridNeighbors<Gases>& neighbors = allNeighbors.at({x, y});
            Gases gasDifference;
            int32_t zoneId = mZones.zones.at(currentTileIndex);
            const Gases& currentGases = mAtmosphere.at(currentTileIndex);
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
            for(size_t i = 0; i < neighborAmount; ++i)
                totalDifference += neighborDifferences[i];

            float transferRate = std::abs(totalDifference / 10000.0f) / neighborAmount;
            transferRate = std::min(0.0625f, std::max(0.05f, transferRate));

            if(totalDifference)
            {
                auto& sourceGasesDifference = mAtmosphereDifference.at(currentTileIndex);
                for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
                {
                    auto& targetGasesDifference = mAtmosphereDifference.at(neighbors.neighbors[neighborIndex].first);
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

            ++currentTileIndex;
        }
    }

    currentTileIndex = 0;
    for(int32_t y = 0; y < mZones.zones.size().y; ++y)
    {
        for(int32_t x = 0; x < mZones.zones.size().x; ++x)
        {
            const Gases& gasDifference = mAtmosphereDifference.at(currentTileIndex);
            Gases& gases = mAtmosphere.at(currentTileIndex);
            for(size_t gasIndex = 0; gasIndex < gasDifference.size(); ++gasIndex)
            {
                gases[gasIndex] += gasDifference[gasIndex];
            }
            ++currentTileIndex;
        }
    }
}
