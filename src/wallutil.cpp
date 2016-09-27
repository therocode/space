#include "wallutil.hpp"
#include <thero/assert.hpp>

glm::vec2 wallCenter(const WallPosition& position)
{
    glm::vec2 result = glm::vec2(position.position * 32);
    result += position.orientation == Orientation::Horizontal ? glm::vec2(16.0f, 0.0f) : glm::vec2(0.0f, 16.0f);
    return result;
}

WallPosition wallBetween(const glm::ivec2& a, const glm::ivec2& b)
{
    TH_ASSERT((a.x == b.x || a.y == b.y) && (std::abs(a.x - b.x) == 1 || std::abs(a.y - b.y) == 1), "invalid arguments " << a << " and " << b);

    if(a.x == b.x)
    {
        return {{a.x, std::max(a.y, b.y)}, Orientation::Horizontal};
    }
    else
    {
        return {{std::max(a.x, b.x), a.y}, Orientation::Vertical};
    }
}
