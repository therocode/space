#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"

class ZoneLogic
{
    public:
        ZoneLogic(const WallMap& walls, Zones& zones);
        void update();
    private:
        void updateZones();

        const WallMap& mWalls;
        Zones& mZones;
};
