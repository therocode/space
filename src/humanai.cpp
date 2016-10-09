#include "humanai.hpp"
#include "taskutil.hpp"
#include "wallutil.hpp"
#include "tileutil.hpp"
#include "pathfindingutil.hpp"
#include "doorutil.hpp"
#include "structures/airlock.hpp"
#include "itemutil.hpp"
#include "moveutil.hpp"
#include "structureutil.hpp"

ActionResult humanGoto(int32_t aiId, int32_t actionId, GameData& data)
{
    GotoAction& gotoAction = get(actionId, data.tGotoAction);
    const glm::vec2& position = get(aiId, data.tPosition);
    glm::ivec2 start = position / 32.0f;
    glm::ivec2 end = gotoAction.target / 32.0f;

    if(!gotoAction.pathId)
    {
        auto path = findWorkerPath(start, end, data); //todo: pass in if you're allowed to get paths through hazard atmosphere
        if(path)
        {
            gotoAction.pathId = path->pathId;

            gotoAction.pathIndex = path->path.path.size() > 1 ? 1 : 0;
        }
        else
        {
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
                if(gotoAction.allowUnbreathable == GotoAction::Disallow)
                {//I need a space suit but I am not allowed to find one
                    return {ActionResult::Fail};
                }

                if(!hasSpaceSuit(aiId, data))
                {
                    const Action& action = get(actionId, data.tAction);
                    gotoAction.pathId = {};
                    gotoAction.pathIndex = {};
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
                                standStill(aiId, data);
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
                    if(glm::distance(position, gotoAction.target) > gotoAction.acceptableDistance)
                    {
                        set(aiId, gotoAction.target, data.tWalkTarget);
                    }
                    else
                    {
                        return {ActionResult::Success};
                    }
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
        int32_t takenTask = closestTask(get(aiId, data.tPosition), data.unassignedTasks, data);
        erase(takenTask, data.unassignedTasks);
        assignTask(takenTask, aiId, data);
        const Task& task = get(takenTask, data.tTask);
        
        return {ActionCreateData{aiId, Ai::Human, takenTask, task.type}};
    }

    return {};
}

ActionResult humanConstructWall(int32_t aiId, int32_t actionId, GameData& data)
{
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
    {
        return tA.actionId == actionId;
    }, data.tTaskAction)->data;

    const auto& targetTile = get(taskAction.taskId, data.tWallTask).position;
    auto targetPosition = wallCenter(targetTile);
    const auto& currentPosition = get(aiId, data.tPosition);
    float acceptableDistance = 15.0f;

    if(glm::distance(currentPosition, targetPosition) > acceptableDistance)
    {
        return {ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{targetPosition, acceptableDistance, {}, {}, GotoAction::Allow}}};
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
    const Action& action = get(actionId, data.tAction);
    const TaskAction& taskAction = findOne([&] (int32_t id, const TaskAction& tA)
    {
        return tA.actionId == actionId;
    }, data.tTaskAction)->data;

    const auto& targetTile = get(taskAction.taskId, data.tDoorTask).position;
    auto targetPosition = wallCenter(targetTile);
    const auto& currentPosition = get(aiId, data.tPosition);
    float acceptableDistance = 15.0f;

    if(glm::distance(currentPosition, targetPosition) > acceptableDistance)
    {
        return {ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{targetPosition, acceptableDistance, {}, {}, GotoAction::Allow}}};
    }
    else
    {
        ConstructDoorAction& constructDoorAction = get(actionId, data.tConstructDoorAction);
        if(constructDoorAction.workLeft > 0)
            --constructDoorAction.workLeft;
        else
        {
            createDoor(Door{{targetTile}}, data);
            return {ActionResult::Success};
        }
    }

    return {};
}

ActionResult humanEquipSpaceSuit(int32_t aiId, int32_t actionId, GameData& data)
{
    const Action& action = get(actionId, data.tAction);

    th::Optional<glm::vec2> itemPosition;
    th::Optional<int32_t> item;
    forEach([&] (int32_t itemId, const Wearable& wearable)
    {
        if(!wearable.wearer && wearable.airTank)
        {
            forEach([&](int32_t, const ItemStoring& itemStoring)
            {
                if(itemStoring.itemId == itemId)
                {
                    itemPosition = containerPosition(itemStoring.containerId, data);
                    item = itemStoring.itemId;
                }
            }, data.tItemStoring);

            if(itemPosition)
                return LoopResult::Break;
            else
                return LoopResult::Continue;
        }

        return LoopResult::Continue;
    }, data.tWearable);

    if(itemPosition)
    {
        const glm::vec2& position = get(aiId, data.tPosition);

        float acceptableDistance = 15.0f;

        if(glm::distance(position, *itemPosition) > acceptableDistance)
        {
            return ActionResult{ActionResult::InProgress, ActionVariant{action.actorId, {actionId}, GotoAction::type, GotoAction{*itemPosition, acceptableDistance, {}, {}, GotoAction::Disallow}}};
        }
        else
        {
            takeOutItem(*item, data);
            putOnWearable(aiId, *item, data);
            return {ActionResult::Success};
        }
    }
    else
    {
        //no free space suit :<
        return {ActionResult::Fail};
    }

    return {};
}
