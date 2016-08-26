#pragma once
#include "data.hpp"
#include "../actor.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class ActorLogic
{
    public:
        ActorLogic(EntityData& ent, GfxData& gfx, TaskData& tsk, WorldData& wld, const WallMap& walls, WallChanges& wallChanges);
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

        EntityData& mEnt;
        GfxData& mGfx;
        TaskData& mTsk;
        WorldData& mWld;
        const WallMap& mWalls;
        WallChanges& mWallChanges;
};
