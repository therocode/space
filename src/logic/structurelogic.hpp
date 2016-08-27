#pragma once
#include "data.hpp"
#include "../resourcemanager.hpp"

class StructureLogic
{
    public:
        StructureLogic(GameData& data, const ResourceManager& resources);
        void update();
    private:
        GameData& mData;
};
