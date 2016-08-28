#pragma once
#include "data.hpp"
#include "../actor.hpp"

class ActorLogic
{
    public:
        ActorLogic(GameData& data);
        void removeActor(int32_t id);
        int32_t addActor(Actor actor);
        void update();
    private:
        void updateDeath();
        void updateWorkers();
        void updateTaskWork();
        void calculateMoveIntention();
        void applyMoveIntention();
        void applyPhysics();
        void applyCollisions();

        GameData& mData;
};
