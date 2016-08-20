#include "atmospherelogic.hpp"
#include "../debug.hpp"

AtmosphereLogic::AtmosphereLogic(const Zones& zones, const WallMap& walls, Grid<Gases>& atmosphere):
    mZones(zones),
    mWalls(walls),
    mAtmosphere(atmosphere),
    mAtmosphereDifference(mAtmosphere.size(), Gases{})
{
}

void AtmosphereLogic::update()
{
    mAtmosphereDifference.fill(Gases{});

    std::array<size_t, 4> neighbors;

    size_t nextRowIndexSkip = static_cast<size_t>(mZones.zones.size().x);
    auto fetchNeighbors = [&] (int32_t id, size_t index, int32_t x, int32_t y, std::array<size_t, 4>& out)
    {
        size_t count = 0;

        if(x > 0 && mZones.zones.at(index - 1) == id && mWalls.at(index, Orientation::Vertical) == 0)
        {
            out[count] = index - 1;
            ++count;
        }
        if(x < static_cast<int32_t>(nextRowIndexSkip) - 1 && mZones.zones.at(index + 1) == id && mWalls.at(index + 1, Orientation::Vertical) == 0)
        {
            out[count] = index + 1;
            ++count;
        }
        if(y > 0 && mZones.zones.at(index - nextRowIndexSkip) == id && mWalls.at(index, Orientation::Horizontal) == 0)
        {
            out[count] = index - nextRowIndexSkip;
            ++count;
        }
        if(y < mZones.zones.size().y - 1 && mZones.zones.at(index + nextRowIndexSkip) == id && mWalls.at(index + nextRowIndexSkip, Orientation::Horizontal) == 0)
        {
            out[count] = index + nextRowIndexSkip;
            ++count;
        }

        return count;
    };

    std::array<int32_t, 4> neighborDifferences;

    size_t currentTileIndex = 0;
    for(int32_t y = 0; y < mZones.zones.size().y; ++y)
    {
        for(int32_t x = 0; x < mZones.zones.size().x; ++x)
        {
            Gases gasDifference;
            int32_t zoneId = mZones.zones.at(currentTileIndex);
            const Gases& currentGases = mAtmosphere.at(currentTileIndex);
            size_t neighborAmount = fetchNeighbors(zoneId, currentTileIndex, x, y, neighbors);
            std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);

            for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
            {
                size_t neighbor = neighbors[neighborIndex];
                const Gases& neighborGases = mAtmosphere.at(neighbor);

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
                for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
                {
                    size_t neighbor = neighbors[neighborIndex];
                    auto& sourceGasesDifference = mAtmosphereDifference.at(currentTileIndex);
                    auto& targetGasesDifference = mAtmosphereDifference.at(neighbor);
                    const Gases& neighborGases = mAtmosphere.at(neighbor);

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
