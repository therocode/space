#pragma once
#include "data.hpp"
#include "../util/grid.hpp"
#include "../gases.hpp"

class OrganismLogic
{
    public:
        OrganismLogic(GameData& data, Grid<Gases>& atmosphere);
        void update();
    private:
        GameData& mData;
        Grid<Gases>& mAtmosphere;
};
