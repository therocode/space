#pragma once
#include "wallposition.hpp"

glm::vec2 wallCenter(const WallPosition& position);
WallPosition wallBetween(const glm::ivec2& a, const glm::ivec2& b);
glm::ivec2 otherSide(const WallPosition& position);
