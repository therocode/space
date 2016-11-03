#pragma once
#include <data.hpp>

class AtmosphereLogic
{
    public:
        AtmosphereLogic(GameData& data);
        void update(const Grid<GridNeighbors<Gases>>& neighbors);
        void scanActive();
    private:
        void updateBigChanges();
        void updatePassive();
        GameData& mData;
        Grid<Gases> mAtmosphereDifference;
        std::vector<size_t> mActive;
};
