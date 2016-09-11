#include "decisionailogic.hpp"
#include "../actionutil.hpp"

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
            int32_t newId = insert(Incentive{id, 0, Incentive::Breathe}, mData.tIncentive);
            insert(newId, BreatheIncentive{}, mData.tBreatheIncentive);

            newId = insert(Incentive{id, 0, Incentive::Work}, mData.tIncentive);
            insert(newId, WorkIncentive{}, mData.tWorkIncentive);
        }
    }, mData.uninitializedAis);
    clear(mData.uninitializedAis);

    forEach([&] (int32_t aiId, const Ai& ai)
    {
		int32_t mostImportantIncentive = -1;
		int32_t mostImportantIncentiveScore = -1;

        forEach([&] (int32_t incentiveId, const Incentive& incentive)
        {
            if(incentive.actorId == aiId && incentive.importance > mostImportantIncentiveScore)
            {
                mostImportantIncentiveScore = incentive.importance;
                mostImportantIncentive = incentiveId;
            }
        }, mData.tIncentive);

        if(!has(mostImportantIncentive, mData.activeIncentives))
        {
            eraseIf([&] (int32_t id)
            {
                return get(id, mData.tIncentive).actorId == aiId;
            },mData.activeIncentives);
            insert(mostImportantIncentive, mData.activeIncentives);
            clearActions(aiId, mData);

            createAction(aiId, ai.type, get(mostImportantIncentive, mData.tIncentive).type, mData);
        }
    }, mData.tAi);
}
