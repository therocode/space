#pragma once
#include "gases.hpp"

const Gases cDefaultAtmosphere
{{
    static_cast<int32_t>(50000 * 0.0f), //oxygene
    static_cast<int32_t>(50000 * 0.0f), //nitrogen
    static_cast<int32_t>(50000 * 1.0f), //carbondioxide
}};
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

bool approxEquals(const Gases& gasA, const Gases& gasB, int64_t tolerance);
bool healthyAtmosphere(const Gases& gas);
