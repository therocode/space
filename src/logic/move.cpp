#include "move.hpp"
#include "../util/accelerator.hpp"

void calculateMoveIntention(const ent::TWalkTarget& tWalkTarget, ent::TMoveIntention& tMoveIntention, const ent::TPosition& tPosition)
{
    join([&] (int32_t id, const glm::vec2& walkTarget, const glm::vec2& position)
    {
        MoveIntention moveIntention;
        moveIntention.direction = glm::normalize(walkTarget - position);
        moveIntention.speedPercent = 0.5f;
        set(id, std::move(moveIntention), tMoveIntention);
    }, tWalkTarget, tPosition);
}

void applyMoveIntention(const ent::TMoveIntention& tMoveIntention, const ent::TMoveAbility& tMoveAbility, ent::TPhysics& tPhysics)
{
    join([] (int32_t id, const MoveIntention& moveIntention, const MoveAbility& moveAbility, Physics& physics)
    {
        float maxSpeed = moveIntention.speedPercent * moveAbility.maxSpeed;

        physics.acceleration = Accelerator::get(moveIntention.direction, maxSpeed, physics.velocity, moveAbility.maxAcceleration);

    }, tMoveIntention, tMoveAbility, tPhysics);
}
