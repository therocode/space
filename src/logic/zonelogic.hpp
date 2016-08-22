#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"

class ZoneLogic
{
    public:
        ZoneLogic(Zones& zones);
        void update(WallMap walls, const WallChanges& changedWalls);
    private:
        void updateZones(WallMap walls, const WallChanges& changedWalls);

        Zones& mZones;
        NumberPool<int32_t> mZoneIds;
};
