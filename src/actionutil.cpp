#include "actionutil.hpp"

void clearActions(int32_t actorId, GameData& data)
{
    eraseIf([&] (int32_t id, const Action& action)
    {
        if(action.actorId == actorId)
        {
            //delete extra data
            return true;
        }
        return false;
    }, data.tAction);
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
            TH_ASSERT(false, "Cannot create action for unknown incentive type");
        }
    }
    else
    {
        TH_ASSERT(false, "Cannot create action for unknown ai type");
    }
}
