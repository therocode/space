#pragma once
#include <thero/optional.hpp>
#include "data.hpp"

enum ActorType { ENGINEER, };

struct Actor
{
    th::Optional<glm::vec2> position;
    th::Optional<Physics> physics;
    th::Optional<CollisionBox> collisionBox;
    th::Optional<MoveAbility> moveAbility;
    th::Optional<BloodValues> bloodValues;
    std::vector<ActorSprite> actorSprites;
    bool worker;
};
