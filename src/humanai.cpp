#include "humanai.hpp"
#include "taskutil.hpp"
#include "wallutil.hpp"
#include "tileutil.hpp"
#include "pathfindingutil.hpp"


th::Optional<ActionVariant> humanGoto(int32_t aiId, int32_t actionId, GameData& data)
{
    GotoAction& gotoAction = get(actionId, data.tGotoAction);
    const glm::vec2& position = get(aiId, data.tPosition);
    glm::ivec2 start = position / 32.0f;
    glm::ivec2 end = gotoAction.target / 32.0f;

    if(!gotoAction.pathId)
    {
        auto path = findWorkerPath(start, end, data);
        if(path)
        {
            gotoAction.pathId = path->pathId;
            gotoAction.pathIndex = 0;
        }
        else
        {
            std::cout << "I can't goto but I dunno how to fail\n";
            //FAIL
        }
    }
    else
    {
        auto* path = findOne(*gotoAction.pathId, data.tPath);

        if(path)
        {
            if(*gotoAction.pathIndex < path->path.size() - 1)
            {
                glm::vec2 nextPos = tileCenter(path->path[*gotoAction.pathIndex]);
                if(glm::distance(position, nextPos) > 5.0f)
                {
                    set(aiId, nextPos, data.tWalkTarget);
                }
                else
                {
                    ++(*gotoAction.pathIndex);
                }
            }
            else
            {
                glm::vec2 nextPos = tileCenter(path->path[*gotoAction.pathIndex]);
                if(glm::distance(position, nextPos) > gotoAction.acceptableDistance)
                {
                    set(aiId, nextPos, data.tWalkTarget);
                }
                else
                {
                    ++(*gotoAction.pathIndex);
                }
            }
        }
        else
        {
            std::cout << "My path expired\n";
            gotoAction.pathId = {};
        }
    }

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
    //check for completion
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
            {
            return tA.actionId == actionId;
            }, data.tTaskAction)->data;

    const auto& position = get(taskAction.taskId, data.tWallTask).position;
    float acceptableDistance = 50.0f;
    
    return {ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{wallCenter(position), acceptableDistance, {}, {}}}};
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
    
    return {ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{wallCenter(position), acceptableDistance, {}, {}}}};
}
