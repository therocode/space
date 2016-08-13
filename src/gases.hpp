#pragma once
#include <cstdint>
#include <array>

enum Gas { Oxygen = 0, Nitrogen, CarbonDioxide, Gas_Count };
using Gases = std::array<int32_t, Gas_Count>;
