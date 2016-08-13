#include "actorlogic.hpp"
#include "../util/accelerator.hpp"
#include "../tableutil.hpp"
#include "../taskutil.hpp"
#include "../debug.hpp"

ActorLogic::ActorLogic(ent::TPosition& tPosition, ent::TPhysics& tPhysics, ent::TMoveAbility& tMoveAbility, ent::TMoveIntention& tMoveIntention, ent::TWalkTarget& tWalkTarget, gfx::TActorSprite& tActorSprite, IdSet& builders, IdSet& freeWorkers, ent::TBusyWorker& tBusyWorker, tsk::TAssignedTask& tAssignedTask, const tsk::TRoomTask& tRoomTask, const tsk::TWallTask& tWallTask, IdSet& unassignedTasks, WallMap& walls):
    mTPosition(tPosition),
    mTPhysics(tPhysics),
    mTMoveAbility(tMoveAbility),
    mTMoveIntention(tMoveIntention),
    mTWalkTarget(tWalkTarget),
    mTActorSprite(tActorSprite),
    mBuilders(builders),
    mFreeWorkers(freeWorkers),
    mTBusyWorker(tBusyWorker),
    mTAssignedTask(tAssignedTask),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask),
    mUnassignedTasks(unassignedTasks),
    mWalls(walls)
{
}

int32_t ActorLogic::addActor(Actor actor)
{
    int32_t id = actor.id;
    tableEmplaceOptional(id, std::move(actor.position), mTPosition);
    tableEmplaceOptional(id, std::move(actor.physics), mTPhysics);
    tableEmplaceOptional(id, std::move(actor.moveAbility), mTMoveAbility);

    for(auto& sprite : actor.actorSprites)
    {   
        tableEmplaceOptional(mActorSpriteIdPool.next(), std::move(sprite), mTActorSprite);
    }   

    if(actor.worker)
    {
        insert(id, mBuilders);
        insert(id, mFreeWorkers);
    }

    return id;
}

void ActorLogic::removeActor(int32_t id)
{
    erase(id, mTPosition);
    erase(id, mTPhysics);
    erase(id, mTMoveAbility);
    erase(id, mTMoveIntention);
    erase(id, mTWalkTarget);

    eraseIf([&] (int32_t actorSpriteId, const ActorSprite& actorSprite)
    {
        if(actorSprite.actorId == id)
        {
            mActorSpriteIdPool.release(actorSpriteId);
            return true;
        }
        return false;
    }, mTActorSprite);
    erase(id, mBuilders);
    erase(id, mFreeWorkers);
}

void ActorLogic::update()
{
    updateWorkers();
    updateTaskWork();
    calculateMoveIntention();
    applyMoveIntention();
    applyPhysics();
}

void ActorLogic::updateWorkers()
{
    //validate that busy workers are not working on a deleted task
    eraseIf([&] (int32_t id, const BusyWorker& worker)
    {
        bool erase = false;
        //if(!has(mT verify that it is in assigned tasks, otherwise insert in free workers and delete
        if(!findOne([&](int32_t taskId, const AssignedTask& assignedTask)
        {
            return assignedTask.assigneeId == id;
        }, mTAssignedTask))
        {
            erase = true;
            insert(id, mFreeWorkers);
        }

        return erase;
    }, mTBusyWorker);

    //assign free workers to unassigned tasks
    eraseIf([&] (int32_t id)
    {
        bool erase = false;
        if(count(mUnassignedTasks) > 0)
        {
            int32_t taskId = extractOne(mUnassignedTasks);
            assignTask(taskId, id, mTAssignedTask);
            insert(id, {taskId}, mTBusyWorker);
            erase = true;
        }

        return erase;
    }, mFreeWorkers);
}

void ActorLogic::updateTaskWork()
{
    forEach([&](const int32_t workerId, const BusyWorker& worker)
    {
        if(auto wallTask = findOne(worker.taskId, mTWallTask))
        {
            glm::vec2 taskPosition = wallTask->data.position * 32;
            const glm::vec2& workerPosition = get(workerId, mTPosition).data;

            if(glm::distance(taskPosition, workerPosition) <= 32.0f)
            {
                erase(workerId, mTWalkTarget);

                if(rand() % 100 == 0)
                {
                    mWalls.set(wallTask->data.position, wallTask->data.orientation, 1);
                }
            }
            else
            {
                set(workerId, {taskPosition}, mTWalkTarget);
            }
        }
        else if(auto roomTask = findOne(worker.taskId, mTRoomTask))
        {
        }
    }, mTBusyWorker);
}

void ActorLogic::calculateMoveIntention()
{
    forEach([&] (int32_t id, MoveIntention& intention)
    {
        if(!has(id, mTWalkTarget))
        {
            intention.speedPercent = 0.0f;
        }
    }, mTMoveIntention);

    join([&] (int32_t id, const glm::vec2& walkTarget, const glm::vec2& position)
    {
        MoveIntention moveIntention;
        moveIntention.direction = glm::normalize(walkTarget - position);
        moveIntention.speedPercent = 0.5f;
        set(id, std::move(moveIntention), mTMoveIntention);
    }, mTWalkTarget, mTPosition);
}

void ActorLogic::applyMoveIntention()
{
    join([] (int32_t id, const MoveIntention& moveIntention, const MoveAbility& moveAbility, Physics& physics)
    {
        float maxSpeed = moveIntention.speedPercent * moveAbility.maxSpeed;

        physics.acceleration = Accelerator::get(moveIntention.direction, maxSpeed, physics.velocity, moveAbility.maxAcceleration);

    }, mTMoveIntention, mTMoveAbility, mTPhysics);
}

void ActorLogic::applyPhysics()
{
    join([] (int32_t id, glm::vec2& position, Physics& physics)
    {
        physics.velocity += physics.acceleration;
        position += physics.velocity;
    }, mTPosition, mTPhysics);
}
