#include "zonelogic.hpp"

ZoneLogic::ZoneLogic(const WallMap& walls, Zones& zones):
    mWalls(walls),
    mZones(zones)
{
}

void ZoneLogic::update()
{
    updateZones();
}

void ZoneLogic::updateZones()
{
    (void)mWalls;
    (void)mZones;
    //grab wall change list and operate on it. maybe grab it by diffing walls? 
}
