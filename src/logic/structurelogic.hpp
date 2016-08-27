#pragma once
#include "data.hpp"
#include "../resourcemanager.hpp"

class StructureLogic
{
    public:
        StructureLogic(StructureData& str, const ResourceManager& resources);
        void update();
    private:
        StructureData& mStr;
};
