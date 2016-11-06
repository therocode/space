#include "atmosphereutil.hpp"
#include "tileutil.hpp"
#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <data.hpp>

bool approxEquals(const Gases& gasA, const Gases& gasB, int64_t tolerance)
{
    for(size_t gasIndex = 0; gasIndex < Gas_Count; ++gasIndex)
    {
        if(std::abs(static_cast<int64_t>(gasA[gasIndex]) - static_cast<int64_t>(gasB[gasIndex])) > tolerance)
            return false;
    }

    return true;
}

bool healthyAtmosphere(const Gases& gas)
{
    //TODO: make more accurate based on what they can brearther
    return approxEquals(gas, cHealthyAtmosphere, 10000);
}

int32_t pressure(const Gases& gases)
{
    return std::accumulate(gases.begin(), gases.end(), 0);
}

float pressurePercent(const Gases& gases)
{
    return pressure(gases) / static_cast<float>(standardPressure);
}

void setAtmosphere(glm::ivec2 tile, const Gases& gases, GameData& data)
{
    data.atmosphere.set(tile, gases);
    data.activeAtmosphereIndices.insert(toIndex(tile, data.atmosphere));
}
