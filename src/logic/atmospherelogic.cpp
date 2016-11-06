#include "atmospherelogic.hpp"
#include "atmosphereutil.hpp"
#include "tileutil.hpp"
#include "../debug.hpp"

AtmosphereLogic::AtmosphereLogic(GameData& data):
    mData(data),
    mRandomGen(0)
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
    mActiveToAdd.clear();
    if(mAtmosphereDifference.size().x == 0)
        mAtmosphereDifference = {mData.atmosphere.size(), Gases{}};

    size_t before = mData.activeAtmosphereIndices.size();

    mAtmosphereDifference.fill(Gases{});

    const GridNeighbors<Gases>* startNeighborsPointer = &allNeighbors.at({0, 0});
    Gases* startGasesPointer = &mData.atmosphere.at({0, 0});
    Gases* startGasesDifferencePointer = &mAtmosphereDifference.at({0, 0});

    std::array<int32_t, 4> neighborDifferences;

    std::vector<PressureDifference> pressureDifferences;
    std::vector<size_t> recentDifferences;
    pressureDifferences.reserve(mData.activeAtmosphereIndices.size());

    //big changes 

    for(size_t activeTileIndex : mData.activeAtmosphereIndices)
    {
        const GridNeighbors<Gases>& neighbors = startNeighborsPointer[activeTileIndex];
        const Gases& currentGases = startGasesPointer[activeTileIndex];
        int32_t totalGas = pressure(currentGases);
        
        size_t neighborAmount = neighbors.neighborCount;
        std::fill(neighborDifferences.begin(), neighborDifferences.end(), 0);
        
        int32_t totalDifference = 0;
        recentDifferences.clear();

        int32_t neighborsWithDifferences = 0;
        for(size_t neighborIndex = 0; neighborIndex < neighborAmount; ++neighborIndex)
        {
            const Gases& neighborGases = *neighbors.neighbors[neighborIndex].second;

            int64_t pressureDifference = totalGas - pressure(neighborGases);
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
                ++neighborsWithDifferences;
            }
        }

        for(size_t diffIndex : recentDifferences)
        {
            auto& diff = pressureDifferences[diffIndex];
            diff.percent = (static_cast<float>(diff.difference) / totalGas) / static_cast<float>(neighborsWithDifferences + 1);
        }
    }

    for(const auto& pressureDifference : pressureDifferences)
    {
        auto& sourceGases = startGasesPointer[pressureDifference.source];
        auto& targetGases = startGasesPointer[pressureDifference.target];
        auto& sourceGasesDifference = startGasesDifferencePointer[pressureDifference.source];
        auto& targetGasesDifference = startGasesDifferencePointer[pressureDifference.target];


        bool didChange = false;

        for(size_t gasIndex = 0; gasIndex < targetGasesDifference.size(); ++gasIndex)
        {
            int32_t toTransfer = static_cast<int32_t>(sourceGases[gasIndex] * pressureDifference.percent);


            if(toTransfer > 0)
            {
                targetGasesDifference[gasIndex] += toTransfer;
                sourceGasesDifference[gasIndex] -= toTransfer;
                didChange = true;
            }
        }

        if(didChange)
            mActiveToAdd.insert(pressureDifference.target);
    }

    //random variations

    std::uniform_int_distribution<> randomPercent(0, 100);
    std::uniform_int_distribution<> randomGas(0, Gas_Count - 1);
    std::uniform_int_distribution<> randomTransferAmount(0, 20);

    for(size_t sourceIndex : mData.activeAtmosphereIndices)
    {
        const GridNeighbors<Gases>& neighbors = startNeighborsPointer[sourceIndex];
        for(size_t neighborIndex = 0; neighborIndex < neighbors.neighborCount; ++ neighborIndex)
        {
            if(randomPercent(mRandomGen) < 10)
            {
                size_t gasIndex = static_cast<size_t>(randomGas(mRandomGen));
                int64_t transferAmount = 5 + randomTransferAmount(mRandomGen);

                size_t targetIndex = neighbors.neighbors[neighborIndex].first;
                int32_t& targetGasDifference = startGasesDifferencePointer[targetIndex][gasIndex];
                int32_t& sourceGasDifference = startGasesDifferencePointer[sourceIndex][gasIndex];
                int64_t targetGasAmount = startGasesPointer[targetIndex][gasIndex] + targetGasDifference;
                int64_t sourceGasAmount = startGasesPointer[sourceIndex][gasIndex] + sourceGasDifference;
                transferAmount = std::min(sourceGasAmount, transferAmount);

                if(transferAmount > 0)
                {
                    targetGasDifference += transferAmount;
                    sourceGasDifference -= transferAmount;
                    mActiveToAdd.insert(targetIndex);
                }
            }
        }
    }

    //apply diff

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

        mActiveToAdd.insert(toIndex(leak.end, mData.atmosphere));
    }, mData.tZoneLeak);

    mData.activeAtmosphereIndices.insert(mActiveToAdd.begin(), mActiveToAdd.end());

    //fade to normal atmos

    std::vector<size_t> toErase;

    for(size_t activeTile : mData.activeAtmosphereIndices)
    {
        if(mData.zones.zones.at(fromIndex(activeTile, mData.atmosphere)) == 0)
        {
            Gases& gases = startGasesPointer[activeTile];

            if(gases != mData.defaultAtmosphere)
            {
                for(size_t gasIndex = 0; gasIndex < Gas_Count; ++gasIndex)
                {
                    int64_t fadeAmount = std::min(10, std::max(-10, mData.defaultAtmosphere[gasIndex] - gases[gasIndex]));

                    gases[gasIndex] += fadeAmount;
                }
            }
            else
                toErase.push_back(activeTile);
        }
    }

    for(size_t erase : toErase)
    {
        mData.activeAtmosphereIndices.erase(erase);
    }

    TH_ASSERT(assertSanity(), "");
}

void AtmosphereLogic::scanActive()
{
    mData.activeAtmosphereIndices.clear();
    const auto& defaultAtmosphere = mData.defaultAtmosphere;
    size_t index = 0;

    for(int32_t y = 0; y < mData.atmosphere.size().y; ++y)
    {
        for(int32_t x = 0; x < mData.atmosphere.size().x; ++x)
        {
            if(mData.atmosphere.at({x, y}) != defaultAtmosphere)
                mData.activeAtmosphereIndices.insert(index);
            ++index;
        }
    }
}

bool AtmosphereLogic::assertSanity()
{
    std::array<int64_t, Gas_Count> amounts = {};

    for(size_t i = 0; i < static_cast<size_t>(mData.atmosphere.size().x * mData.atmosphere.size().y); ++i)
    {
        for(size_t gasIndex = 0; gasIndex < Gas_Count; ++gasIndex)
        {
            TH_ASSERT(mData.atmosphere.at(i)[gasIndex] >= 0, "Negative gas!!!");
        }
    }

    return true;
}
