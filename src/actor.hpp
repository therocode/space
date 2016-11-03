#pragma once
#include <thero/optional.hpp>
#include "data.hpp"

enum ActorType { ENGINEER, };

struct Actor
{
    Position position;
    th::Optional<Physics> physics;
    th::Optional<CollisionBox> collisionBox;
    th::Optional<MoveAbility> moveAbility;
    th::Optional<BloodValues> bloodValues;
    std::vector<ActorSprite> actorSprites;
    bool worker;
    th::Optional<AiType> ai;
};
