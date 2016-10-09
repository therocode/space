#include "atmosphereutil.hpp"
#include <cstdint>

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
