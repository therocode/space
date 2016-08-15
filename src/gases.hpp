#pragma once
#include <cstdint>
#include <array>

enum Gas { Oxygen = 0, Nitrogen, CarbonDioxide, Gas_Count };
using Gases = std::array<int32_t, Gas_Count>;

constexpr int32_t standardPressure = static_cast<int32_t>(50000.0 * 0.209f + 50000.0 * 0.78f + 50000.0 * 0.004f);

int32_t pressure(const Gases& gases);
float pressurePercent(const Gases& gases);
