#include "actorlogic.hpp"
#include "../util/accelerator.hpp"
#include "../tableutil.hpp"
#include "../taskutil.hpp"
#include "../doorutil.hpp"
#include "../debug.hpp"

ActorLogic::ActorLogic(EntityData& ent, GfxData& gfx, TaskData& tsk, WorldData& wld, WallMap& walls):
    mEnt(ent),
    mGfx(gfx),
    mTsk(tsk),
    mWld(wld),
    mWalls(walls)
{
}

int32_t ActorLogic::addActor(Actor actor)
{
    int32_t id = *tableEmplaceOptional(std::move(actor.position), mEnt.tPosition);
    tableEmplaceOptional(id, std::move(actor.physics), mEnt.tPhysics);
    tableEmplaceOptional(id, std::move(actor.moveAbility), mEnt.tMoveAbility);
    tableEmplaceOptional(id, std::move(actor.bloodValues), mEnt.tBloodValues);

    for(auto& sprite : actor.actorSprites)
    {   
        sprite.actorId = id;
        insert(std::move(sprite), mGfx.tActorSprite);
    }   

    if(actor.worker)
    {
        insert(id, mEnt.builders);
        insert(id, mEnt.freeWorkers);
    }

    return id;
}

void ActorLogic::removeActor(int32_t id)
{
    erase(id, mEnt.tPosition);
    erase(id, mEnt.tPhysics);
    erase(id, mEnt.tMoveAbility);
    erase(id, mEnt.tMoveIntention);
    erase(id, mEnt.tWalkTarget);
    erase(id, mEnt.tBloodValues);

    eraseIf([&] (int32_t actorSpriteId, const ActorSprite& actorSprite)
    {
        if(actorSprite.actorId == id)
        {
            return true;
        }
        return false;
    }, mGfx.tActorSprite);
    erase(id, mEnt.builders);
    erase(id, mEnt.freeWorkers);
}

void ActorLogic::update()
{
    updateDeath();
    updateWorkers();
    updateTaskWork();
    calculateMoveIntention();
    applyMoveIntention();
    applyPhysics();
}

void ActorLogic::updateDeath()
{
    forEach([&] (int32_t id)
    {
        erase(id, mEnt.tPhysics);
        erase(id, mEnt.tWalkTarget);
        erase(id, mEnt.tMoveIntention);
        erase(id, mEnt.tMoveAbility);

        erase(id, mEnt.builders);
        erase(id, mEnt.freeWorkers);

        if(auto busyWorker = findOne(id, mEnt.tBusyWorker))
        {
            int32_t taskId = busyWorker->data.taskId;
            erase(taskId, mTsk.tAssignedTask);
            insert(taskId, mTsk.unassignedTasks);
            erase(id, mEnt.tBusyWorker);
        }

        erase(id, mEnt.tBloodValues);
        erase(id, mEnt.tChoking);

        forEach([&] (int32_t spriteId, ActorSprite& sprite)
        {
            if(sprite.actorId == id)
                sprite.color = fea::Color(60, 70, 40);
        }, mGfx.tActorSprite);

        insert(id, mEnt.deadWorkers);
    }, mEnt.died);
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
        }, mTsk.tAssignedTask))
        {
            erase = true;
            insert(id, mEnt.freeWorkers);
        }

        return erase;
    }, mEnt.tBusyWorker);

    //assign free workers to unassigned tasks
    eraseIf([&] (int32_t id)
    {
        bool erase = false;
        if(count(mTsk.unassignedTasks) > 0)
        {
            int32_t taskId = extractOne(mTsk.unassignedTasks);
            assignTask(taskId, id, mTsk.tAssignedTask);
            insert(id, {taskId}, mEnt.tBusyWorker);
            erase = true;
        }

        return erase;
    }, mEnt.freeWorkers);
}

void ActorLogic::updateTaskWork()
{
    forEach([&](const int32_t workerId, const BusyWorker& worker)
    {
        if(auto wallTask = findOne(worker.taskId, mTsk.tWallTask))
        {
            glm::vec2 taskPosition = wallTask->data.position * 32;
            const glm::vec2& workerPosition = get(workerId, mEnt.tPosition);

            if(glm::distance(taskPosition, workerPosition) <= 32.0f)
            {
                erase(workerId, mEnt.tWalkTarget);

                if(rand() % 100 == 0)
                {
                    mWalls.set(wallTask->data.position, wallTask->data.orientation, 1);
                }
            }
            else
            {
                set(workerId, {taskPosition}, mEnt.tWalkTarget);
            }
        }
        else if(auto roomTask = findOne(worker.taskId, mTsk.tRoomTask))
        {
        }
        else if(auto doorTask = findOne(worker.taskId, mTsk.tDoorTask))
        {
            glm::vec2 taskPosition = doorTask->data.position * 32;
            const glm::vec2& workerPosition = get(workerId, mEnt.tPosition);

            if(glm::distance(taskPosition, workerPosition) <= 32.0f)
            {
                erase(workerId, mEnt.tWalkTarget);

                if(rand() % 100 == 0)
                {
                    createDoor(Door{doorTask->data.position, doorTask->data.orientation}, mWld.tDoor, mWld.openDoors, mWalls);
                }
            }
            else
            {
                set(workerId, {taskPosition}, mEnt.tWalkTarget);
            }
        }
    }, mEnt.tBusyWorker);
}

void ActorLogic::calculateMoveIntention()
{
    forEach([&] (int32_t id, MoveIntention& intention)
    {
        if(!has(id, mEnt.tWalkTarget))
        {
            intention.speedPercent = 0.0f;
        }
    }, mEnt.tMoveIntention);

    forEach([&] (int32_t id)
    {
    }, mEnt.died);

    join([&] (int32_t id, const glm::vec2& walkTarget, const glm::vec2& position)
    {
        MoveIntention moveIntention;
        moveIntention.direction = glm::normalize(walkTarget - position);
        moveIntention.speedPercent = 0.5f;
        set(id, std::move(moveIntention), mEnt.tMoveIntention);
    }, mEnt.tWalkTarget, mEnt.tPosition);
}

void ActorLogic::applyMoveIntention()
{
    join([] (int32_t id, const MoveIntention& moveIntention, const MoveAbility& moveAbility, Physics& physics)
    {
        float maxSpeed = moveIntention.speedPercent * moveAbility.maxSpeed;

        physics.acceleration = Accelerator::get(moveIntention.direction, maxSpeed, physics.velocity, moveAbility.maxAcceleration);

    }, mEnt.tMoveIntention, mEnt.tMoveAbility, mEnt.tPhysics);
}

void ActorLogic::applyPhysics()
{
    join([] (int32_t id, glm::vec2& position, Physics& physics)
    {
        physics.velocity += physics.acceleration;
        position += physics.velocity;
    }, mEnt.tPosition, mEnt.tPhysics);
}
