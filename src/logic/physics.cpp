#include "physics.hpp"

void applyPhysics(ent::TPosition& tPosition, ent::TPhysics& tPhysics)
{
    join([] (int32_t id, glm::vec2& position, Physics& physics)
    {
        physics.velocity += physics.acceleration;
        position += physics.velocity;
    }, tPosition, tPhysics);
}
