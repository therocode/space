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

namespace ent
{
    using TPosition  = DataTable<glm::vec2>;
}

namespace gfx
{
    using TActorSprite = DataTable<ActorSprite>;
}
