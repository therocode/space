#pragma once
#include "../util/grid.hpp"
#include "../gases.hpp"
#include "../zones.hpp"
#include "../wallmap.hpp"
#include "../gridneighbors.hpp"

class AtmosphereLogic
{
    public:
        AtmosphereLogic(const Zones& zones, const WallMap& walls, Grid<Gases>& atmosphere);
        void update(const Grid<GridNeighbors<Gases>>& neighbors);
    private:
        const Zones& mZones;
        const WallMap& mWalls;
        Grid<Gases>& mAtmosphere;
        Grid<Gases> mAtmosphereDifference;
};
