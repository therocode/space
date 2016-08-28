#pragma once
#include <data.hpp>

class OrganismLogic
{
    public:
        OrganismLogic(GameData& data);
        void update();
    private:
        GameData& mData;
};
