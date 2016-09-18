#include "wallutil.hpp"

glm::vec2 wallCenter(const WallPosition& position)
{
    glm::vec2 result = glm::vec2(position.position * 32);
    result += position.orientation == Orientation::Horizontal ? glm::vec2(16.0f, 0.0f) : glm::vec2(0.0f, 16.0f);
    return result;
}
