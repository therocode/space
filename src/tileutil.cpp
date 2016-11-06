#include "tileutil.hpp"

glm::vec2 tileCenter(glm::ivec2 tile)
{
    return static_cast<glm::vec2>(tile * 32) + glm::vec2(16.0f, 16.0f);
}
