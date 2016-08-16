#pragma once
#include "data.hpp"
#include "../util/grid.hpp"
#include "../gases.hpp"

class OrganismLogic
{
    public:
        OrganismLogic(const ent::TPosition& tPosition, ent::TBloodValues& tBloodValues, Grid<Gases>& atmosphere);
        void update();
    private:
        const ent::TPosition& mTPosition;
        ent::TBloodValues& mTBloodValues;
        Grid<Gases>& mAtmosphere;
};
