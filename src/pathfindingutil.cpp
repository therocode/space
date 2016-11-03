#include "pathfindingutil.hpp"
#include "wallutil.hpp"

th::Optional<PathEntry> findWorkerPath(const glm::ivec2& start, const glm::ivec2& end, GameData& data, int32_t maxCost)
{
    fea::Pathfinder<WorkerPathAdaptor> pathfinder;
    WorkerPathAdaptor adaptor(data);

    auto pathEntry = findOne([&](int32_t id, const Path& path)
    {
        return path.path.path.front() == start && path.path.path.back() == end;
    }, data.tPath);

    if(pathEntry)
    {
        return {PathEntry{pathEntry->id, pathEntry->data}};
    }
    else
    {
        Path path = {pathfinder.findPath(adaptor, start, end)};

        if(path.path.path.size() > 0)
        {
            auto added  = insert(std::move(path), data.tPath);

            return {PathEntry{added.id, added.data}};
        }
        else
            return {};
    }
}

bool wallObstructsPath(int32_t pathId, WallPosition wallPosition, GameData& data)
{
    const auto& path = get(pathId, data.tPath).path.path;

    if(path.size() < 2)
        return false;

    for(size_t i = 0; i < path.size() - 1; ++i)
    {
        const auto& start = path[i];
        const auto& end = path[i + 1];
        if(wallBetween(start, end) == wallPosition)
            return true;
    }

    return false;
}

void invalidatePaths(GameData& data)
{
    for(auto wallChanges : data.wallChanges)
    {
        WallPosition wallPosition = wallChanges.first;
        eraseIf([&] (int32_t id, const Path& path)
        {
            return wallObstructsPath(id, wallPosition, data);
        }, data.tPath);
    }
}

th::Optional<int32_t> findWorkerPathCost(const glm::vec2& start, const glm::vec2& end, GameData& data)
{
    auto path = findWorkerPath(start, end, data);

    if(path)
    {
        return path->path.path.cost;
    }
    else
        return {};
}
