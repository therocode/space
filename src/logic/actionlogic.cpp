#include "actionlogic.hpp"
#include "../actionutil.hpp"

ActionLogic::ActionLogic(GameData& data):
    mData(data)
{
    (void)mData;
}

void ActionLogic::update()
{
    forEach([&] (int32_t actionId, const TotalPanicAction& totalPanicAction)
    {
        const Action& action = get(actionId, mData.tAction);
        auto newTarget = get(action.actorId, mData.tPosition) + glm::circularRand(10.0f);
        set(action.actorId, newTarget, mData.tWalkTarget);
    }, mData.tTotalPanicAction);
}
