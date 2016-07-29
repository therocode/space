#pragma once
#include <thero/optional.hpp>
#include <data.hpp>
#include "data.hpp"

enum ActorType { ENGINEER, };

struct Actor
{
    int32_t id;
    th::Optional<glm::vec2> position;
    th::Optional<Physics> physics;
    th::Optional<MoveAbility> moveAbility;
    std::vector<ActorSprite> actorSprites;
    bool worker;
};
