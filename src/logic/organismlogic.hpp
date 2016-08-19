#pragma once
#include "data.hpp"
#include "../util/grid.hpp"
#include "../gases.hpp"

class OrganismLogic
{
    public:
        OrganismLogic(EntityData& ent, Grid<Gases>& atmosphere);
        void update();
    private:
        EntityData& mEnt;
        Grid<Gases>& mAtmosphere;
};
