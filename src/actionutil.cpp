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
