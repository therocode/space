#pragma once
#include <thero/optional.hpp>
#include <data.hpp>
#include "wallposition.hpp"

glm::vec2 wallCenter(const WallPosition& position);
WallPosition wallBetween(const glm::ivec2& a, const glm::ivec2& b);
glm::ivec2 otherSide(const WallPosition& position);
th::Optional<WallPosition> positionToWall(const glm::vec2& position, float thickness);
bool hasWall(const WallPosition& position, const GameData& data);
