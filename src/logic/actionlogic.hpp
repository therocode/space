#pragma once
#include <data.hpp>

class ActionLogic
{
    public:
        ActionLogic(GameData& data);
        void update();
    private:
        GameData& mData;
};
