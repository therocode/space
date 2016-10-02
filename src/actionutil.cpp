#include "actionutil.hpp"
#include "incentiveutil.hpp"
#include "aiutil.hpp"
#include "taskutil.hpp"

void clearActions(int32_t actorId, GameData& data)
{
    std::vector<int32_t> toDelete;

    forEach([&] (int32_t id, const Action& action)
    {
        if(action.actorId == actorId)
        {
            toDelete.push_back(id);
        }
    }, data.tAction);

    for(int32_t actionId : toDelete)
        deleteAction(actionId, data);
}

void createAction(int32_t aiId, Ai::Type aiType, Incentive::Type incentiveType, GameData& data)
{
    if(aiType == Ai::Human)
    {
        if(incentiveType == Incentive::Breathe)
        {
            addAction(aiId, TotalPanicAction{}, data.tTotalPanicAction, data);
        }
        else if(incentiveType == Incentive::Work)
        {
            addAction(aiId, FindWorkTaskAction{}, data.tFindWorkTaskAction, data);
        }
        else
        {
            TH_ASSERT(false, "Cannot create action for unknown incentive type " << toString(incentiveType));
        }
    }
    else
    {
        TH_ASSERT(false, "Cannot create action for unknown ai type " << toString(aiType));
    }
}

void createAction(int32_t aiId, Ai::Type aiType, int32_t taskId, Task::Type taskType, GameData& data)
{
    if(aiType == Ai::Human)
    {
        if(taskType == Task::Door)
        {
            int32_t newActionId = addAction(aiId, ConstructDoorAction{100}, data.tConstructDoorAction, data);
            insert({newActionId, taskId}, data.tTaskAction);
        }
        else if(taskType == Task::Wall)
        {
            int32_t newActionId = addAction(aiId, ConstructWallAction{100}, data.tConstructWallAction, data);
            insert({newActionId, taskId}, data.tTaskAction);
        }
        else
        {
            TH_ASSERT(false, "Cannot create action for unknown task type " << toString(taskType));
        }
    }
    else
    {
        TH_ASSERT(false, "Cannot create action for unknown ai type " << toString(aiType));
    }
}

std::string toString(Action::Type type)
{
    if(type == Action::Goto)
        return {"Goto"};
    else if(type == Action::TotalPanic)
        return {"Total Panic"};
    else if(type == Action::FindWorkTask)
        return {"Find work task"};
    else if(type == Action::ConstructWall)
        return {"Construct Wall"};
    else if(type == Action::ConstructDoor)
        return {"Construct Door"};
    else if(type == Action::EquipSpaceSuit)
        return {"Equip space suit"};
    else
    {
        TH_ASSERT(false, "unknown action");
        return "bleh";
    }
}

void deleteAction(int32_t actionId, GameData& data)
{
    const Action& action = get(actionId, data.tAction);
    erase(actionId, data.leafActions);
    if(action.type == Action::TotalPanic)
        erase(actionId, data.tTotalPanicAction);
    else if(action.type == Action::Goto)
        erase(actionId, data.tGotoAction);
    else if(action.type == Action::FindWorkTask)
        erase(actionId, data.tFindWorkTaskAction);
    else if(action.type == Action::ConstructWall)
        erase(actionId, data.tConstructWallAction);
    else if(action.type == Action::ConstructDoor)
        erase(actionId, data.tConstructDoorAction);
    else if(action.type == Action::EquipSpaceSuit)
        erase(actionId, data.tEquipSpaceSuitAction);
    else
    {
        TH_ASSERT(false, "No cleanup for action " << toString(action.type));
    }

    //What! How to remember to put stuff here?

    auto foundTaskAction = findOne([&] (int32_t taskActionId, const TaskAction& taskAction)
            {
            return taskAction.actionId == actionId;
            }, data.tTaskAction);

    if(foundTaskAction)
    {
        int32_t taskId = foundTaskAction->data.taskId;

        erase(taskId, data.tAssignedTask);
        insert(taskId, data.unassignedTasks);

        erase(action.actorId, data.tBusyWorker);
        insert(action.actorId, data.freeWorkers);

        erase(foundTaskAction->id, data.tTaskAction);      
    }
    erase(action.actorId, data.tWalkTarget);
    erase(actionId, data.tAction);
}

void deleteLeafAction(int32_t leafActionId, GameData& data)
{
    erase(leafActionId, data.leafActions);
    const Action& action = get(leafActionId, data.tAction);
    if(action.parentAction)
        insert(*action.parentAction, data.leafActions);
    deleteAction(leafActionId, data);
}
