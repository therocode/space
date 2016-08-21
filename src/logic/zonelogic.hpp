#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"
#include "../wallutil.hpp"

class ZoneLogic
{
    public:
        ZoneLogic(Zones& zones);
        void update(WallMap walls, const std::vector<WallChange>& changedWalls);
    private:
        void updateZones(WallMap walls, const std::vector<WallChange>& changedWalls);

        Zones& mZones;
        NumberPool<int32_t> mZoneIds;
};
