#include "humanai.hpp"
#include "taskutil.hpp"
#include "wallutil.hpp"
#include "tileutil.hpp"
#include "pathfindingutil.hpp"
#include "doorutil.hpp"
#include "structures/airlock.hpp"
#include "itemutil.hpp"
#include "structureutil.hpp"


ActionResult humanGoto(int32_t aiId, int32_t actionId, GameData& data)
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
            //std::cout << "I can't goto but I dunno how to fail\n";
            return {ActionResult::Fail};
        }
    }
    else
    {
        auto* path = findOne(*gotoAction.pathId, data.tPath);

        if(path)
        {
            if(needsSpaceSuit(*path, data))
            {
                if(!hasSpaceSuit(aiId, data))
                {
                    const Action& action = get(actionId, data.tAction);
                    return {ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, EquipSpaceSuitAction::type, EquipSpaceSuitAction{}}};
                }
            }

            if(*gotoAction.pathIndex < path->path.size() - 1)
            {
                const glm::ivec2& nextTile = path->path[*gotoAction.pathIndex];
                glm::vec2 nextPos = tileCenter(nextTile);
                glm::ivec2 currentTile = static_cast<glm::ivec2>(position / 32.0f);

                //std::cout << "next: " << nextTile << " current: " << currentTile << "\n";
                if(nextTile != currentTile)
                {
                    WallPosition doorPosition = wallBetween(currentTile, nextTile);

                    //std::cout << "checking for door at " << doorPosition.position << " " << to_string(doorPosition.orientation) << "\n";
                    th::Optional<int32_t> lockedDoorId = lockedDoorAt(doorPosition, data);

                    if(lockedDoorId)
                    {
                        auto doorLockEntry = findOne([&] (int32_t id, const StructureDoorLock& structureDoorLock)
                        {
                            return *lockedDoorId == structureDoorLock.doorId;
                        }, data.tStructureDoorLock);

                        //std::cout << "locked door in the way: " << *lockedDoorId << "\n";
                        if(doorLockEntry)
                        {
                            //std::cout << "structure door in the way: " << *lockedDoorId << "\n";
                            int32_t structureId = doorLockEntry->data.structureId;
                            const Structure& structure = get(structureId, data.tStructure);
                            
                            if(structure.type == Structure::Airlock)
                            {
                                if(!has(structureId, data.tAirlockActivity))
                                {
                                    requestOpenAirlockDoor(structureId, *lockedDoorId, data);
                                }

                                return {};
                            }
                            else
                            {
                                //no idea how to open this door
                                return {ActionResult::Fail};
                            }
                        ////if owned by airlock
                        ////toggle airlock and set that mode
                            
                        }
                        else
                        {
                            //door locked by other means than structure... just fail
                            return {ActionResult::Fail};
                        }
                    }
                }

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
                    return {ActionResult::Success};
                }
            }
        }
        else
        {
            //std::cout << "My path expired\n";
            gotoAction.pathId = {};
            gotoAction.pathIndex = {};
        }
    }

    return {};
}

ActionResult humanTotalPanic(int32_t aiId, int32_t actionId, GameData& data)
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

ActionResult humanConstructWall(int32_t aiId, int32_t actionId, GameData& data)
{
    //check for completion
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
            {
            return tA.actionId == actionId;
            }, data.tTaskAction)->data;

    const auto& targetTile = get(taskAction.taskId, data.tWallTask).position;
    auto targetPosition = wallCenter(targetTile);
    const auto& currentPosition = get(aiId, data.tPosition);
    float acceptableDistance = 50.0f;

    if(glm::distance(currentPosition, targetPosition) > acceptableDistance)
    {
        return {ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{targetPosition, acceptableDistance, {}, {}}}};
    }
    else
    {
        ConstructWallAction& constructWallAction = get(actionId, data.tConstructWallAction);
        if(constructWallAction.workLeft > 0)
            --constructWallAction.workLeft;
        else
        {
            set(targetTile, 1, data.walls, data.wallChanges);
            return {ActionResult::Success};
        }
    }

    return {};
}

ActionResult humanConstructDoor(int32_t aiId, int32_t actionId, GameData& data)
{
    //check for completion
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
            {
            return tA.actionId == actionId;
            }, data.tTaskAction)->data;

    const auto& position = get(taskAction.taskId, data.tDoorTask).position;
    float acceptableDistance = 50.0f;
    
    return {ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{wallCenter(position), acceptableDistance, {}, {}}}};
}

ActionResult humanEquipSpaceSuit(int32_t aiId, int32_t actionId, GameData& data)
{
    ActionResult result;

    const Action& action = get(actionId, data.tAction);
    forEach([&] (int32_t itemId, const Wearable& wearable)
    {
        if(!wearable.wearer && wearable.airTank)
        {
            th::Optional<glm::vec2> itemPosition;
            forEach([&](int32_t, const ItemStoring& itemStoring)
            {
                if(itemStoring.itemId == itemId)
                {
                    itemPosition = containerPosition(itemStoring.containerId, data);
                }
            }, data.tItemStoring);

            float acceptableDistance = 50.0f;

            if(itemPosition)
                result = ActionResult{ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{*itemPosition, acceptableDistance, {}, {}}}};
        }
    }, data.tWearable);
    return result;
}
