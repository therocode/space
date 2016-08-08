#pragma once
#include "data.hpp"
#include "../actor.hpp"
#include "../util/numberpool.hpp"

class ActorLogic
{
    public:
        ActorLogic(ent::TPosition& tPosition, ent::TPhysics& tPhysics, ent::TMoveAbility& tMoveAbility, ent::TMoveIntention& tMoveIntention, ent::TWalkTarget& tWalkTarget, gfx::TActorSprite& tActorSprite, IdSet& builders, IdSet& freeWorkers);
        void removeActor(int32_t id);
        int32_t addActor(Actor actor);
        void update();
    private:
        void calculateMoveIntention();
        void applyMoveIntention();
        void applyPhysics();

        NumberPool<int32_t> mActorSpriteIdPool;
        ent::TPosition& mTPosition;
        ent::TPhysics& mTPhysics;
        ent::TMoveAbility& mTMoveAbility;
        ent::TMoveIntention& mTMoveIntention;
        ent::TWalkTarget& mTWalkTarget;
        gfx::TActorSprite& mTActorSprite;
        IdSet& mBuilders;
        IdSet& mFreeWorkers;
};
