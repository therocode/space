#include "move.hpp"
#include "../util/accelerator.hpp"

void applyMoveIntention(const ent::TMoveIntention& tMoveIntention, const ent::TMoveAbility& tMoveAbility, ent::TPhysics& tPhysics)
{
    join([] (int32_t id, const MoveIntention& moveIntention, const MoveAbility& moveAbility, Physics& physics)
    {
        float maxSpeed = moveIntention.speedPercent * moveAbility.maxSpeed;

        physics.acceleration = Accelerator::get(moveIntention.direction, maxSpeed, physics.velocity, moveAbility.maxAcceleration);

    }, tMoveIntention, tMoveAbility, tPhysics);
}
