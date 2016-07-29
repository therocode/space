#pragma once
#include <cstdint>
#include <vector>
#include "common.hpp"
#include "join.hpp"
#include "insert.hpp"
#include "find.hpp"
#include "get.hpp"
#include "foreach.hpp"
#include "erase.hpp"
#include "clear.hpp"
#include "has.hpp"
//includes
#include "glm/glm.hpp"
#include <fea/rendering/color.hpp>

//table layouts
struct ActorSprite
{
    int32_t actorId;
    int32_t textureId;
    fea::Color color;
};

struct Physics
{
    glm::vec2 acceleration;
    glm::vec2 velocity;
};

struct MoveIntention
{
    glm::vec2 direction;
    float speedPercent;
};

struct MoveAbility
{
    float maxSpeed;
    float maxAcceleration;
};

namespace ent
{
    using TPosition  = DataTable<glm::vec2>;
    using TPhysics  = DataTable<Physics>;
    using TWalkTarget = DataTable<glm::vec2>;
    using TMoveIntention = DataTable<MoveIntention>;
    using TMoveAbility = DataTable<MoveAbility>;
}

namespace gfx
{
    using TActorSprite = DataTable<ActorSprite>;
}
