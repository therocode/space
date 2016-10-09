#include "moveutil.hpp"

void standStill(int32_t actorId, GameData& data)
{
    erase(actorId, data.tWalkTarget);
}
