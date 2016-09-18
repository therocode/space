#include "actionlogic.hpp"
#include "../actionutil.hpp"
#include "../workerpathadaptor.hpp"
#include <fea/util.hpp>
#include "../taskutil.hpp"
#include "../wallutil.hpp"

ActionLogic::ActionLogic(GameData& data):
    mData(data)
{
    (void)mData;
}

struct NewTaskAction
{
    int32_t aiId;
    Ai::Type aiType;
    int32_t taskId;
    Task::Type taskType;
};

void ActionLogic::update()
{
    forEach([&] (int32_t actionId)
    {
        const Action& action = get(actionId, mData.tAction);
        const Ai& ai = get(action.actorId, mData.tAi);

        if(ai.type == Ai::Human)
        {
            //solve actions here. Actions must be able to create actions without adding them in this loop since iterator invalidation
        }
        else
        {
        }
              
    }, mData.leafActions);

    forEach([&] (int32_t actionId, const TotalPanicAction& totalPanicAction)
    {
        if(rand() % 10 == 0)
        {
            const Action& action = get(actionId, mData.tAction);
            auto newTarget = get(action.actorId, mData.tPosition) + glm::circularRand(10.0f);
            set(action.actorId, newTarget, mData.tWalkTarget);
        }
    }, mData.tTotalPanicAction);

    std::vector<NewTaskAction> newActions;
    forEach([&] (int32_t actionId, const FindWorkTaskAction& findWorkTaskAction)
    {
        //fea::Pathfinder<WorkerPathAdaptor> pathfinder;
        //WorkerPathAdaptor adaptor(mData);

        //auto path = pathfinder.findPath(adaptor, {8, 7}, {10, 7});

        //if(!path.path.empty())
        //{
        //}

        if(count(mData.unassignedTasks) > 0)
        {
            const Action& action = get(actionId, mData.tAction);
            const Ai& ai = get(action.actorId, mData.tAi);
            int32_t takenTask = extractOne(mData.unassignedTasks);

            assignTask(takenTask, action.actorId, mData);

            const Task& task = get(takenTask, mData.tTask);
            
            newActions.push_back({action.actorId, ai.type, takenTask, task.type});
        }
    }, mData.tFindWorkTaskAction);

    for(const auto& newTaskAction : newActions)
    {
        clearActions(newTaskAction.aiId, mData);
        createAction(newTaskAction.aiId, newTaskAction.aiType, newTaskAction.taskId, newTaskAction.taskType, mData);
    }

    forEach([&] (int32_t actionId, const ConstructDoorAction& constructDoorAction)
    {
        //check for completion
        const Action& action = get(actionId, mData.tAction);
        const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
        {
            return tA.actionId == actionId;
        }, mData.tTaskAction)->data;
        std::cout << "will get " << taskAction.taskId << " from door task table\n";
        const auto& position = get(taskAction.taskId, mData.tDoorTask).position;
        float acceptableDistance = 50.0f;
        addChildAction(action.actorId, actionId, GotoAction{wallCenter(position), acceptableDistance}, mData.tGotoAction, mData);  
    }, mData.tConstructDoorAction);
}
