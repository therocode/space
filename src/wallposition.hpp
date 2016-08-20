#pragma once
#include <functional>
#include "glm.hpp"
#include "orientation.hpp"

struct WallPosition
{
    glm::ivec2 position;
    Orientation orientation;
};

bool operator==(const WallPosition& a, const WallPosition& b);

namespace std
{
  template <> struct hash<WallPosition>
  {
    size_t operator()(const WallPosition& wallPos) const
    {
        return (hash<glm::ivec2>()(wallPos.position) << 32) | static_cast<size_t>(wallPos.orientation);
    }
  };
}
