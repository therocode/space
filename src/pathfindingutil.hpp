#pragma once
#include <data.hpp>

struct PathEntry
{
    int32_t pathId;
    Path& path;
};

th::Optional<PathEntry> findWorkerPath(const glm::ivec2& start, const glm::ivec2& end, GameData& data, int32_t maxCost = -1);
bool wallObstructsPath(int32_t pathId, WallPosition wallPosition, GameData& data);
void invalidatePaths(GameData& data);
