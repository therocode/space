#include "atmospherelogic.hpp"
#include "atmosphereutil.hpp"
#include "../debug.hpp"

AtmosphereLogic::AtmosphereLogic(GameData& data):
    mData(data)
{
}

struct PressureDifference
{
    size_t source;
    size_t target;
    int64_t difference;
    float percent;
};

void AtmosphereLogic::update(const Grid<GridNeighbors<Gases>>& allNeighbors)
{
    (void)mData;
    updateBigChanges();
    updatePassive();
    if(mAtmosphereDifference.size().x == 0)
        mAtmosphereDifference = {mData.atmosphere.size(), Gases{}};

    mAtmosphereDifference.fill(Gases{});

    const GridNeighbors<Gases>* startNeighborsPointer = &allNeighbors.at({0, 0});
    Gases* startGasesPointer = &mData.atmosphere.at({0, 0});
    Gases* startGasesDifferencePointer = &mAtmosphereDifference.at({0, 0});

    std::array<int32_t, 4> neighborDifferences;

    std::vector<PressureDifference> pressureDifferences;
    std::vector<size_t> recentDifferences;
    pressureDifferences.reserve(mActive.size());

    for(size_t activeTileIndex : mActive)
    {
        const GridNeighbors<Gases>& neighbors = startNeighborsPointer[activeTileIndex];
        const Gases& currentGases = startGasesPointer[activeTileIndex];
        
        size_t neighborAmount = neighbors.neighborCount;
        std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);
        
        int32_t totalDifference = 0;
        recentDifferences.clear();
        for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
        {
            const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

            int64_t pressureDifference = pressure(currentGases) - pressure(neighborGases);
            if(pressureDifference > 0)
            {
                pressureDifferences.push_back(PressureDifference
                {
                    activeTileIndex,
                    neighbors.neighbors[neighborIndex].first,
                    pressureDifference,
                    0.0f,
                });
                totalDifference += pressureDifferences.back().difference;
                recentDifferences.push_back(pressureDifferences.size() - 1);
            }
        }

        for(size_t diffIndex : recentDifferences)
        {
            auto& diff = pressureDifferences[diffIndex];
            diff.percent = (static_cast<float>(diff.difference) / totalDifference) / 4.0f;
        }
    }

    for(const auto& pressureDifference : pressureDifferences)
    {
        auto& sourceGases = startGasesPointer[pressureDifference.source];
        auto& targetGases = startGasesPointer[pressureDifference.target];
        auto& sourceGasesDifference = startGasesDifferencePointer[pressureDifference.source];
        auto& targetGasesDifference = startGasesDifferencePointer[pressureDifference.target];

        for(size_t gasIndex = 0; gasIndex < targetGasesDifference.size(); ++gasIndex)
        {
            int32_t toTransfer = static_cast<int32_t>((sourceGases[gasIndex] - targetGases[gasIndex]) * pressureDifference.percent);

            if(toTransfer > 0)
            {
                targetGasesDifference[gasIndex] += toTransfer;
                sourceGasesDifference[gasIndex] -= toTransfer;
            }
        }
    }
       
    //for every neighbour
    //   fill list of differences in pressure, storing 2 tiles, source and target and write to a second structure how big the difference is or something
    //
    //for every tile in list of differences in pressure
    //   calculate how much to spread and write differences to mAtmosphereDifference
    //
    //apply mAtmosphereDifference

    //const int32_t* startZoneIdPointer = &mData.zones.zones.at({0, 0});
    //size_t amount = static_cast<size_t>(mData.zones.zones.size().x * mData.zones.zones.size().y);
    //for(size_t currentTileIndex = 0; currentTileIndex < amount; ++currentTileIndex)
    //{
    //    const GridNeighbors<Gases>& neighbors = startNeighborsPointer[currentTileIndex];
    //    Gases gasDifference;
    //    int32_t zoneId = startZoneIdPointer[currentTileIndex];
    //    const Gases& currentGases = startGasesPointer[currentTileIndex];
    //    size_t neighborAmount = neighbors.neighborCount;
    //    std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);

    //    for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
    //    {
    //        const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

    //        for(size_t gasIndex = 0; gasIndex < currentGases.size(); ++gasIndex)
    //        {
    //            neighborDifferences[neighborIndex] += (currentGases[gasIndex] - neighborGases[gasIndex]);
    //        }
    //    }

    //    int32_t totalDifference = 0;
    //    for(size_t differenceIndex = 0; differenceIndex < neighborAmount; ++differenceIndex)
    //        totalDifference += neighborDifferences[differenceIndex];

    //    float transferRate = std::abs(totalDifference / 10000.0f) / neighborAmount;
    //    transferRate = std::min(0.0625f, std::max(0.05f, transferRate));

    //    if(totalDifference)
    //    {
    //        auto& sourceGasesDifference = startGasesDifferencePointer[currentTileIndex];
    //        for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
    //        {
    //            auto& targetGasesDifference = startGasesDifferencePointer[neighbors.neighbors[neighborIndex].first];
    //            const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

    //            if(neighborDifferences[neighborIndex])
    //            {
    //                for(size_t gasIndex = 0; gasIndex < targetGasesDifference.size(); ++gasIndex)
    //                {
    //                    int32_t toTransfer = static_cast<int32_t>((currentGases[gasIndex] - neighborGases[gasIndex]) * transferRate);
    //                    if(toTransfer > 0)
    //                    {
    //                        targetGasesDifference[gasIndex] += toTransfer;
    //                        sourceGasesDifference[gasIndex] -= toTransfer;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    size_t amount = static_cast<size_t>(mData.zones.zones.size().x * mData.zones.zones.size().y);
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

void AtmosphereLogic::scanActive()
{
    mActive.clear();
    const auto& defaultAtmosphere = mData.defaultAtmosphere;
    size_t index = 0;
    for(int32_t y = 0; y < mData.atmosphere.size().y; ++y)
    {
        for(int32_t x = 0; x < mData.atmosphere.size().x; ++x)
        {
            if(mData.atmosphere.at({x, y}) != defaultAtmosphere)
                mActive.push_back(index);
            ++index;
        }
    }
}

void AtmosphereLogic::updateBigChanges()
{
}

void AtmosphereLogic::updatePassive()
{
}
