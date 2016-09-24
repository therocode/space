#include "actionlogic.hpp"
#include "../actionutil.hpp"
#include "../workerpathadaptor.hpp"
#include <fea/util.hpp>
#include "../humanai.hpp"

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
    std::vector<ActionVariant> newActions;
    std::vector<ActionCreateData> replacementActions;

    forEach([&] (int32_t actionId)
    {
        const Action& action = get(actionId, mData.tAction);
        const Ai& ai = get(action.actorId, mData.tAi);

        if(ai.type == Ai::Human)
        {
            //solve actions here. Actions must be able to create actions without adding them in this loop since iterator invalidation
            if(action.type == Action::Goto)
            {
                auto newAction = humanGoto(action.actorId, actionId, mData);
                if(newAction)
                    newActions.push_back(std::move(*newAction));
            }
            else if(action.type == Action::TotalPanic)
            {
                auto newAction = humanTotalPanic(action.actorId, actionId, mData);
                if(newAction)
                    newActions.push_back(std::move(*newAction));
            }
            else if(action.type == Action::FindWorkTask)
            {
                auto replacementAction = humanFindWorkTask(action.actorId, actionId, mData);
                if(replacementAction)
                    replacementActions.push_back(std::move(*replacementAction));
            }
            else if(action.type == Action::ConstructWall)
            {
                auto newAction = humanConstructWall(action.actorId, actionId, mData);
                if(newAction)
                    newActions.push_back(std::move(*newAction));
            }
            else if(action.type == Action::ConstructDoor)
            {
                auto newAction = humanConstructDoor(action.actorId, actionId, mData);
                if(newAction)
                    newActions.push_back(std::move(*newAction));
            }
            else
            {
                TH_ASSERT(false, "unimplemented action: " << toString(action.type));
            }
        }
        else
        {
        }
              
    }, mData.leafActions);

    for(const ActionVariant& newAction : newActions)
    {
        if(newAction.type == Action::Goto)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<GotoAction>()), mData.tGotoAction, mData);  
        }
        else if(newAction.type == Action::TotalPanic)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<TotalPanicAction>()), mData.tTotalPanicAction, mData);  
        }
        else if(newAction.type == Action::FindWorkTask)
        {
            TH_ASSERT(false, "can never be childtask");
        }
        else if(newAction.type == Action::ConstructWall)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<ConstructWallAction>()), mData.tConstructWallAction, mData);  
        }
        else if(newAction.type == Action::ConstructDoor)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<ConstructDoorAction>()), mData.tConstructDoorAction, mData);  
        }
        else
        {
            TH_ASSERT(false, "unimplemented action: " << toString(newAction.type));
        }
    }

    for(const ActionCreateData& replacementAction : replacementActions)
    {
        clearActions(replacementAction.actorId, mData);
        createAction(replacementAction.actorId, replacementAction.aiType, replacementAction.taskId, replacementAction.taskType, mData);
    }
}
