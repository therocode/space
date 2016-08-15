#include "gases.hpp"
#include <numeric>

int32_t pressure(const Gases& gases)
{
    return std::accumulate(gases.begin(), gases.end(), 0);
}

float pressurePercent(const Gases& gases)
{
    return pressure(gases) / static_cast<float>(standardPressure);
}
