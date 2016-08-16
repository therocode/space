#pragma once
#include "data.hpp"
#include "../actor.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class ActorLogic
{
    public:
        ActorLogic(ent::TPosition& tPosition, ent::TPhysics& tPhysics, ent::TMoveAbility& tMoveAbility, ent::TMoveIntention& tMoveIntention, ent::TWalkTarget& tWalkTarget, ent::TBloodValues& tBloodValues, gfx::TActorSprite& tActorSprite, IdSet& builders, IdSet& freeWorkers, ent::TBusyWorker& tBusyWorker, tsk::TAssignedTask& tAssignedTask, const tsk::TRoomTask& tRoomTask, const tsk::TWallTask& tWallTask, IdSet& unassignedTasks, WallMap& walls);
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
        ent::TPosition& mTPosition;
        ent::TPhysics& mTPhysics;
        ent::TMoveAbility& mTMoveAbility;
        ent::TMoveIntention& mTMoveIntention;
        ent::TWalkTarget& mTWalkTarget;
        ent::TBloodValues& mTBloodValues;
        gfx::TActorSprite& mTActorSprite;
        IdSet& mBuilders;
        IdSet& mFreeWorkers;
        ent::TBusyWorker& mTBusyWorker;
        tsk::TAssignedTask& mTAssignedTask;
        const tsk::TRoomTask& mTRoomTask;//move out later
        const tsk::TWallTask& mTWallTask;//move out later
        IdSet& mUnassignedTasks;
        WallMap& mWalls;
};
