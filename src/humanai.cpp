#include "humanai.hpp"
#include "taskutil.hpp"
#include "wallutil.hpp"


th::Optional<ActionVariant> humanGoto(int32_t aiId, int32_t actionId, GameData& data)
{
    return {};
}

th::Optional<ActionVariant> humanTotalPanic(int32_t aiId, int32_t actionId, GameData& data)
{
    if(rand() % 10 == 0)
    {
        auto newTarget = get(aiId, data.tPosition) + glm::circularRand(10.0f);
        set(aiId, newTarget, data.tWalkTarget);
    }

    return {};
}

th::Optional<ActionCreateData> humanFindWorkTask(int32_t aiId, int32_t actionId, GameData& data)
{
    //fea::Pathfinder<WorkerPathAdaptor> pathfinder;
    //WorkerPathAdaptor adaptor(mData);

    //auto path = pathfinder.findPath(adaptor, {8, 7}, {10, 7});

    //if(!path.path.empty())
    //{
    //}

    if(count(data.unassignedTasks) > 0)
    {
        int32_t takenTask = extractOne(data.unassignedTasks);
        assignTask(takenTask, aiId, data);
        const Task& task = get(takenTask, data.tTask);
        
        return {ActionCreateData{aiId, Ai::Human, takenTask, task.type}};
    }

    return {};
}

th::Optional<ActionVariant> humanConstructWall(int32_t aiId, int32_t actionId, GameData& data)
{
    return {};
}

th::Optional<ActionVariant> humanConstructDoor(int32_t aiId, int32_t actionId, GameData& data)
{
    //check for completion
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
            {
            return tA.actionId == actionId;
            }, data.tTaskAction)->data;

    const auto& position = get(taskAction.taskId, data.tDoorTask).position;
    float acceptableDistance = 50.0f;
    
    return {ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{wallCenter(position), acceptableDistance}}};
}
