#include "decisionailogic.hpp"

DecisionAILogic::DecisionAILogic(GameData& data):
    mData(data)
{
    (void)mData;
}

void DecisionAILogic::update()
{
    //initialize uninitialized ones
    forEach([&] (int32_t id)
    {
        if(has(id, mData.humanAis))
        {
            int32_t newId = insert(Incentive{id, 0}, mData.tIncentive);
            insert(newId, BreatheIncentive{}, mData.tBreatheIncentive);

            newId = insert(Incentive{id, 0}, mData.tIncentive);
            insert(newId, WorkIncentive{}, mData.tWorkIncentive);
        }
    }, mData.uninitializedAis);
    clear(mData.uninitializedAis);

    forEach([&] (int32_t id, const Ai& ai)
    {
        //find the incentive which has the most important
        //assign as active
        //if it was a change, delete actions
    }, mData.tAi);
}
