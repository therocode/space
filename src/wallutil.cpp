#include "wallutil.hpp"
#include <thero/assert.hpp>

std::vector<WallChange> wallDiff(const WallMap& oldWalls, const WallMap& newWalls)
{
    TH_ASSERT(oldWalls.size() == newWalls.size(), "non matching sizes: " << oldWalls.size() << " " << newWalls.size());
    std::vector<WallChange> result;

    for(int32_t y = 0; y < oldWalls.size().y; ++y)
    {
        for(int32_t x = 0; x < oldWalls.size().x; ++x)
        {
            if(oldWalls.at({x, y}, Orientation::Horizontal) != newWalls.at({x, y}, Orientation::Horizontal))
                result.push_back(WallChange{{x, y}, Orientation::Horizontal, newWalls.at({x, y}, Orientation::Horizontal)});
            if(oldWalls.at({x, y}, Orientation::Vertical) != newWalls.at({x, y}, Orientation::Vertical))
                result.push_back(WallChange{{x, y}, Orientation::Vertical, newWalls.at({x, y}, Orientation::Vertical)});
        }
    }

    return result;
}
