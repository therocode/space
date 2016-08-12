#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"
#include "../wallutil.hpp"

class ZoneLogic
{
    public:
        ZoneLogic(const WallMap& walls, Zones& zones);
        void update(const std::vector<WallChange>& changedWalls);
    private:
        void updateZones(const std::vector<WallChange>& changedWalls);

        const WallMap& mWalls;
        Zones& mZones;
        int32_t mNextZoneId;
};
