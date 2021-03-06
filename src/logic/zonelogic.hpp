#pragma once
#include "data.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"

class ZoneLogic
{
    public:
        ZoneLogic(GameData& data);
        void update(WallMap& walls, const WallChanges& changedWalls);
    private:
        void updateZones(WallMap& walls, const WallChanges& changedWalls);

        GameData& mData;
        NumberPool<int32_t> mZoneIds;
};
