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

    std::vector<glm::ivec2> neighbors;

    auto fetchNeighbors = [&] (int32_t id, const glm::ivec2& coordinate, std::vector<glm::ivec2>& out)
    {
        out.resize(4);
        size_t count = 0;

        if(coordinate.x > 0 && mZones.zones.at(coordinate + glm::ivec2(-1, 0)) == id && mWalls.at(coordinate, Orientation::Vertical) == 0)
        {
            out[count] = coordinate + glm::ivec2(-1, 0);
            ++count;
        }
        if(coordinate.x < mZones.zones.size().x - 1 && mZones.zones.at(coordinate + glm::ivec2(1, 0)) == id && mWalls.at(coordinate + glm::ivec2(1, 0), Orientation::Vertical) == 0)
        {
            out[count] = coordinate + glm::ivec2(1, 0);
            ++count;
        }
        if(coordinate.y > 0 && mZones.zones.at(coordinate + glm::ivec2(0, -1)) == id && mWalls.at(coordinate, Orientation::Horizontal) == 0)
        {
            out[count] = coordinate + glm::ivec2(0, -1);
            ++count;
        }
        if(coordinate.y < mZones.zones.size().y - 1 && mZones.zones.at(coordinate + glm::ivec2(0, 1)) == id && mWalls.at(coordinate + glm::ivec2(0, 1), Orientation::Horizontal) == 0)
        {
            out[count] = coordinate + glm::ivec2(0, 1);
            ++count;
        }

        out.resize(count);
    };

    std::vector<int32_t> neighborDifferences;

    for(int32_t y = 0; y < mZones.zones.size().y; ++y)
    {
        for(int32_t x = 0; x < mZones.zones.size().x; ++x)
        {
            if(rand() % 2)
                continue;
            Gases gasDifference;
            int32_t zoneId = mZones.zones.at({x, y});
            const Gases& currentGases = mAtmosphere.at({x, y});
            fetchNeighbors(zoneId, {x, y}, neighbors);
            size_t neighborAmount = neighbors.size();
            neighborDifferences.resize(neighborAmount);
            std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);

            for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
            {
                const auto& neighborCoordinate = neighbors[neighborIndex];
                const Gases& neighborGases = mAtmosphere.at(neighborCoordinate);

                for(size_t gasIndex = 0; gasIndex < currentGases.size(); ++gasIndex)
                {
                    neighborDifferences[neighborIndex] += (currentGases[gasIndex] - neighborGases[gasIndex]);
                }
            }

            int32_t totalDifference = std::accumulate(neighborDifferences.begin(), neighborDifferences.end(), 0);

            float transferRate = std::abs(totalDifference / 10000.0f) / neighborAmount;
            transferRate = std::min(0.0625f, std::max(0.05f, transferRate));

            if(totalDifference)
            {
                for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
                {
                    const auto& neighborCoordinate = neighbors[neighborIndex];
                    auto& sourceGasesDifference = mAtmosphereDifference.at({x, y});
                    auto& targetGasesDifference = mAtmosphereDifference.at(neighbors[neighborIndex]);
                    const Gases& neighborGases = mAtmosphere.at(neighborCoordinate);

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
    }

    for(int32_t y = 0; y < mZones.zones.size().y; ++y)
    {
        for(int32_t x = 0; x < mZones.zones.size().x; ++x)
        {
            const Gases& gasDifference = mAtmosphereDifference.at({x, y});
            Gases& gases = mAtmosphere.at({x, y});
            for(size_t gasIndex = 0; gasIndex < gasDifference.size(); ++gasIndex)
            {
                gases[gasIndex] += gasDifference[gasIndex];
            }
        }
    }
}
