#pragma once
#include "../glm.hpp"

class Accelerator
{
    public:
        static glm::vec2 get(glm::vec2 direction, float maxSpeed, const glm::vec2& currentVelocity, float maxAcc);
};
