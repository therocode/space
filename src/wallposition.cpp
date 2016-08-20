#include "wallposition.hpp"

bool operator==(const WallPosition& a, const WallPosition& b)
{
    return a.position == b.position && a.orientation == b.orientation;
}
