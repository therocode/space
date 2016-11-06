#pragma once
#include <data.hpp>
#include <random>

class AtmosphereLogic
{
    public:
        AtmosphereLogic(GameData& data);
        void update(const Grid<GridNeighbors<Gases>>& neighbors);
        void scanActive();
    private:
        bool assertSanity();
        GameData& mData;
        std::mt19937 mRandomGen;
        Grid<Gases> mAtmosphereDifference;
        std::set<size_t> mActiveToAdd;
};
