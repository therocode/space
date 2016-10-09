#include "atmosphereutil.hpp"
#include <cstdint>
#include <cstdlib>

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
    approxEquals(gas, cHealthyAtmosphere, 1000);
    return true;
}
