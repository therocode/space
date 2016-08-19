#pragma once
#include "data.hpp"
#include "../actor.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class ActorLogic
{
    public:
        ActorLogic(EntityData& ent, GfxData& gfx, TaskData& tsk, WallMap& walls);
        void removeActor(int32_t id);
        int32_t addActor(Actor actor);
        void update();
    private:
        void updateWorkers();
        void updateTaskWork();
        void calculateMoveIntention();
        void applyMoveIntention();
        void applyPhysics();

        NumberPool<int32_t> mActorSpriteIdPool;
        EntityData& mEnt;
        GfxData& mGfx;
        TaskData& mTsk;
        WallMap& mWalls;
};
