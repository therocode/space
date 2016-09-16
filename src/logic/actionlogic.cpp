#include "actionlogic.hpp"
#include "../actionutil.hpp"
#include "../workerpathadaptor.hpp"
#include <fea/util.hpp>

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

    forEach([&] (int32_t actionId, const FindWorkTaskAction& findWorkTaskAction)
    {
        fea::Pathfinder<WorkerPathAdaptor> pathfinder;
        WorkerPathAdaptor adaptor(mData);

        auto path = pathfinder.findPath(adaptor, {8, 7}, {10, 7});

        if(!path.path.empty())
        {
        }
    }, mData.tFindWorkTaskAction);
}
