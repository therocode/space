#pragma once
#include <data.hpp>

struct PathEntry
{
    int32_t pathId;
    Path& path;
};

th::Optional<PathEntry> findWorkerPath(const glm::ivec2& start, const glm::ivec2& end, GameData& data, int32_t maxCost = -1);
