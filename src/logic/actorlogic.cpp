#include "actorlogic.hpp"
#include "../util/accelerator.hpp"
#include "../tableutil.hpp"
#include "../taskutil.hpp"
#include "../doorutil.hpp"
#include "../debug.hpp"
#include "../actionutil.hpp"

ActorLogic::ActorLogic(GameData& data):
    mData(data)
{
}

int32_t ActorLogic::addActor(Actor actor)
{
    int32_t id = insert(actor.position, mData.tPosition).id;
    tableEmplaceOptional(id, std::move(actor.physics), mData.tPhysics);
    tableEmplaceOptional(id, std::move(actor.collisionBox), mData.tCollisionBox);
    tableEmplaceOptional(id, std::move(actor.moveAbility), mData.tMoveAbility);
    tableEmplaceOptional(id, std::move(actor.bloodValues), mData.tBloodValues);

    for(auto& sprite : actor.actorSprites)
    {   
        sprite.actorId = id;
        insert(std::move(sprite), mData.tActorSprite);
    }   

    if(actor.worker)
    {
        insert(id, mData.builders);
        insert(id, mData.freeWorkers);
    }

    if(actor.ai)
    {
        Ai newAi;

        if(*actor.ai == AiType::Human)
        {
            newAi.type = AiType::Human;
            insert(id, mData.humanAis);
        }
        insert(id, std::move(newAi), mData.tAi);
        insert(id, mData.uninitializedAis);
    }

    return id;
}

void ActorLogic::removeActor(int32_t id)
{
    erase(id, mData.tPosition);
    erase(id, mData.tPhysics);
    erase(id, mData.tCollisionBox);
    erase(id, mData.tMoveAbility);
    erase(id, mData.tMoveIntention);
    erase(id, mData.tWalkTarget);
    erase(id, mData.tBloodValues);

    eraseIf([&] (int32_t actorSpriteId, const ActorSprite& actorSprite)
    {
        if(actorSprite.actorId == id)
        {
            return true;
        }
        return false;
    }, mData.tActorSprite);
    erase(id, mData.builders);
    erase(id, mData.freeWorkers);
    erase(id, mData.tAi);
    erase(id, mData.humanAis);
    erase(id, mData.uninitializedAis);
    eraseIf([&] (int32_t incentiveId, const Incentive& incentive)
    {
        if(incentive.actorId == id)
        {
            erase(incentiveId, mData.tBreatheIncentive);
            erase(incentiveId, mData.tWorkIncentive);
            erase(incentiveId, mData.activeIncentives);
            return true;
        }
        return false;
    }, mData.tIncentive);
}

void ActorLogic::update()
{
    updateDeath();
    updateWorkers();
    calculateMoveIntention();
    applyMoveIntention();
    applyPhysics();
    applyCollisions();
}

void ActorLogic::updateDeath()
{
    forEach([&] (int32_t id)
    {
        erase(id, mData.tPhysics);
        erase(id, mData.tCollisionBox);
        erase(id, mData.tWalkTarget);
        erase(id, mData.tMoveIntention);
        erase(id, mData.tMoveAbility);

        erase(id, mData.builders);
        erase(id, mData.freeWorkers);

        if(auto busyWorker = findOne(id, mData.tBusyWorker))
        {
            int32_t taskId = busyWorker->taskId;
            erase(taskId, mData.tAssignedTask);
            insert(taskId, mData.unassignedTasks);
            erase(id, mData.tBusyWorker);
        }

        erase(id, mData.tBloodValues);
        erase(id, mData.tChoking);

        clearActions(id, mData);
        erase(id, mData.tAi);
        erase(id, mData.humanAis);
        erase(id, mData.uninitializedAis);
        eraseIf([&] (int32_t incentiveId, const Incentive& incentive)
        {
            if(incentive.actorId == id)
            {
                erase(incentiveId, mData.tBreatheIncentive);
                erase(incentiveId, mData.tWorkIncentive);
                erase(incentiveId, mData.activeIncentives);
                return true;
            }

            return false;
        }, mData.tIncentive);

        forEach([&] (int32_t spriteId, ActorSprite& sprite)
        {
            if(sprite.actorId == id)
                sprite.color = fea::Color(60, 70, 40);
        }, mData.tActorSprite);

        insert(id, mData.deadWorkers);
    }, mData.died);
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
        }, mData.tAssignedTask))
        {
            erase = true;
            insert(id, mData.freeWorkers);
        }

        return erase;
    }, mData.tBusyWorker);

    forEach([&] (int32_t actorId)
    {
        auto foundIncentive = findOne([&] (int32_t incentiveId, const Incentive& incentive)
        {
            if(has(incentiveId, mData.tWorkIncentive))
            {
                return incentive.actorId == actorId;
            }
            return false;
        }, mData.tIncentive);

        if(foundIncentive)
            foundIncentive->data.importance = 50;
    }, mData.freeWorkers);
}

void ActorLogic::calculateMoveIntention()
{
    forEach([&] (int32_t id, MoveIntention& intention)
    {
        if(!has(id, mData.tWalkTarget))
        {
            intention.speedPercent = 0.0f;
        }
    }, mData.tMoveIntention);

    forEach([&] (int32_t id)
    {
    }, mData.died);

    join([&] (int32_t id, const WalkTarget& walkTarget, const Position& position)
    {
        if(glm::distance(walkTarget.position, position.position) > 5.0f)
        {
            MoveIntention moveIntention;
            moveIntention.direction = glm::normalize(walkTarget.position - position.position);
            moveIntention.speedPercent = 0.5f;
            set(id, std::move(moveIntention), mData.tMoveIntention);
        }
    }, mData.tWalkTarget, mData.tPosition);
}

void ActorLogic::applyMoveIntention()
{
    join([] (int32_t id, const MoveIntention& moveIntention, const MoveAbility& moveAbility, Physics& physics)
    {
        float maxSpeed = moveIntention.speedPercent * moveAbility.maxSpeed;

        physics.acceleration = Accelerator::get(moveIntention.direction, maxSpeed, physics.velocity, moveAbility.maxAcceleration);

    }, mData.tMoveIntention, mData.tMoveAbility, mData.tPhysics);
}

void ActorLogic::applyPhysics()
{
    join([] (int32_t id, Position& position, Physics& physics)
    {
        physics.velocity += physics.acceleration;
        position.position += physics.velocity;
    }, mData.tPosition, mData.tPhysics);
}

void ActorLogic::applyCollisions()
{
    join([&] (int32_t id, Position& positionStruct, Physics& physics, const CollisionBox& collisionBox)
    {
        glm::vec2& position = positionStruct.position;
        glm::ivec2 currentTile = position / 32.0f;

        glm::vec2 min = position - collisionBox.size / 2.0f;
        glm::vec2 max = position + collisionBox.size / 2.0f;

        int32_t tileL = static_cast<int32_t>(min.x / 32.0f);
        int32_t tileR = static_cast<int32_t>(max.x / 32.0f);
        int32_t tileT = static_cast<int32_t>(min.y / 32.0f);
        int32_t tileB = static_cast<int32_t>(max.y / 32.0f);

        if(physics.velocity.x < 0.0f && tileL != currentTile.x)
        {
            if(mData.walls.atV(currentTile) != 0)
            {
                auto collidedDoor = findOne([&] (int32_t doorId, const Door& door)
                {
                    return door.position.position == glm::ivec2(currentTile.x, currentTile.y) && door.position.orientation == Orientation::Vertical;
                }, mData.tDoor);

                if(!collidedDoor || has(collidedDoor->id, mData.lockedDoors))
                {
                    physics.velocity.x = 0.0f;
                    position.x = currentTile.x * 32.0f + collisionBox.size.x / 2.0f + 0.001f;
                }
                else
                {
                    openDoor(collidedDoor->id, mData);
                }
            }
        }
        else if(physics.velocity.x > 0.0f && tileR != currentTile.x)
        {
            if(mData.walls.atV({currentTile.x + 1, currentTile.y}) != 0)
            {
                auto collidedDoor = findOne([&] (int32_t doorId, const Door& door)
                {
                    return door.position.position == glm::ivec2(currentTile.x + 1, currentTile.y) && door.position.orientation == Orientation::Vertical;
                }, mData.tDoor);

                if(!collidedDoor || has(collidedDoor->id, mData.lockedDoors))
                {
                    physics.velocity.x = 0.0f;
                    position.x = (currentTile.x + 1) * 32.0f - collisionBox.size.x / 2.0f - 0.001f;
                }
                else
                {
                    openDoor(collidedDoor->id, mData);
                }
            }
        }

        if(physics.velocity.y < 0.0f && tileT != currentTile.y)
        {
            if(mData.walls.atH(currentTile) != 0)
            {
                auto collidedDoor = findOne([&] (int32_t doorId, const Door& door)
                {
                    return door.position.position == glm::ivec2(currentTile.x, currentTile.y) && door.position.orientation == Orientation::Horizontal;
                }, mData.tDoor);

                if(!collidedDoor || has(collidedDoor->id, mData.lockedDoors))
                {
                    physics.velocity.y = 0.0f;
                    position.y = currentTile.y * 32.0f + collisionBox.size.y / 2.0f + 0.001f;
                }
                else
                {
                    openDoor(collidedDoor->id, mData);
                }
            }
        }
        else if(physics.velocity.y > 0.0f && tileB != currentTile.y)
        {
            if(mData.walls.atH({currentTile.x, currentTile.y + 1}) != 0)
            {
                auto collidedDoor = findOne([&] (int32_t doorId, const Door& door)
                {
                    return door.position.position == glm::ivec2(currentTile.x, currentTile.y + 1) && door.position.orientation == Orientation::Horizontal;
                }, mData.tDoor);

                if(!collidedDoor || has(collidedDoor->id, mData.lockedDoors))
                {
                    physics.velocity.y = 0.0f;
                    position.y = (currentTile.y + 1) * 32.0f - collisionBox.size.y / 2.0f - 0.001f;
                }
                else
                {
                    openDoor(collidedDoor->id, mData);
                }
            }
        }

    }, mData.tPosition, mData.tPhysics, mData.tCollisionBox);
}
