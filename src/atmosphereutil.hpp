#pragma once
#include <data.hpp>

const Gases cHealthyAtmosphere
{{
    static_cast<int32_t>(50000 * 0.209f), //oxygene
    static_cast<int32_t>(50000 * 0.78f), //nitrogen
    static_cast<int32_t>(50000 * 0.004f), //carbondioxide
}};
const Gases cWtfAtmosphere
{{
    static_cast<int32_t>(200000000 * 0.00f), //oxygene
    static_cast<int32_t>(200000000 * 0.4f), //nitrogen
    static_cast<int32_t>(200000000 * 0.9f), //carbondioxide
}};

constexpr int32_t standardPressure = static_cast<int32_t>(50000.0 * 0.209f + 50000.0 * 0.78f + 50000.0 * 0.004f);

bool approxEquals(const Gases& gasA, const Gases& gasB, int64_t tolerance);
bool healthyAtmosphere(const Gases& gas);

int32_t pressure(const Gases& gases);
float pressurePercent(const Gases& gases);
void setAtmosphere(glm::ivec2 tile, const Gases& gases, GameData& data);
