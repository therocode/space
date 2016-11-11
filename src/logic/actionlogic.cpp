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
    AiType aiType;
    int32_t taskId;
    Task::Type taskType;
};

void ActionLogic::update()
{
    std::vector<ActionVariant> newActions;
    std::vector<ActionCreateData> replacementActions;
    std::vector<int32_t> doneActions;

    forEach([&] (int32_t actionId)
    {
        const Action& action = get(actionId, mData.tAction);
        const Ai& ai = get(action.actorId, mData.tAi);

        if(ai.type == AiType::Human)
        {
            //solve actions here. Actions must be able to create actions without adding them in this loop since iterator invalidation
            if(action.type == Action::GotoAction)
            {
                auto result = humanGoto(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
            }
            else if(action.type == Action::TotalPanicAction)
            {
                auto result = humanTotalPanic(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
            }
            else if(action.type == Action::FindWorkTaskAction)
            {
                auto replacementAction = humanFindWorkTask(action.actorId, actionId, mData);
                if(replacementAction)
                    replacementActions.push_back(std::move(*replacementAction));
            }
            else if(action.type == Action::ConstructWallAction)
            {
                auto result = humanConstructWall(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
            }
            else if(action.type == Action::ConstructDoorAction)
            {
                auto result = humanConstructDoor(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
            }
            else if(action.type == Action::ConstructStructureAction)
            {
                auto result = humanConstructStructure(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
            }
            else if(action.type == Action::EquipSpaceSuitAction)
            {
                auto result = humanEquipSpaceSuit(action.actorId, actionId, mData);
                if(result.status == ActionResult::Fail || result.status == ActionResult::Success)
                    doneActions.push_back(actionId);
                else if(result.createdSubAction)
                    newActions.push_back(std::move(*result.createdSubAction));
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

    for(int32_t doneAction : doneActions)
    {
        deleteLeafAction(doneAction, mData);
    }

    for(const ActionVariant& newAction : newActions)
    {
        if(newAction.type == Action::GotoAction)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<GotoAction>()), mData.tGotoAction, mData);  
        }
        else if(newAction.type == Action::TotalPanicAction)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<TotalPanicAction>()), mData.tTotalPanicAction, mData);  
        }
        else if(newAction.type == Action::FindWorkTaskAction)
        {
            TH_ASSERT(false, "can never be childtask");
        }
        else if(newAction.type == Action::ConstructWallAction)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<ConstructWallAction>()), mData.tConstructWallAction, mData);  
        }
        else if(newAction.type == Action::ConstructDoorAction)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<ConstructDoorAction>()), mData.tConstructDoorAction, mData);  
        }
        else if(newAction.type == Action::EquipSpaceSuitAction)
        {
            addChildAction(newAction.actorId, *newAction.parentActionId, std::move(newAction.actionData.get<EquipSpaceSuitAction>()), mData.tEquipSpaceSuitAction, mData);  
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
