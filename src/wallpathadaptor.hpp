#pragma once
#include "wallmap.hpp"

class WallPathAdaptor
{
    public:
        WallPathAdaptor(const WallMap& walls);
        const glm::ivec2 getNeighbor(const glm::ivec2& tile, uint32_t index) const;
        uint32_t getNeighborAmount(const glm::ivec2& tile) const;
        int32_t getStepCost(const glm::ivec2& tileA, const glm::ivec2& tileB) const;
        int32_t estimateDistance(const glm::ivec2& start, const glm::ivec2& target) const;
    private:
        const WallMap& mWalls;
};
