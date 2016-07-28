#pragma once
#include <thero/optional.hpp>
#include <data.hpp>
#include "data.hpp"

enum ObjectType { ENGINEER, };

struct Object
{
    int32_t id;
    th::Optional<glm::vec2> position;
    std::vector<ActorSprite> actorSprites;
};
