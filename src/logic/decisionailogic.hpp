#pragma once
#include <data.hpp>

class DecisionAILogic
{
    public:
        DecisionAILogic(GameData& data);
        void update();
    private:
        GameData& mData;
};
