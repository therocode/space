#include "pathfindingutil.hpp"

th::Optional<PathEntry> findWorkerPath(const glm::ivec2& start, const glm::ivec2& end, GameData& data, int32_t maxCost)
{
    fea::Pathfinder<WorkerPathAdaptor> pathfinder;
    WorkerPathAdaptor adaptor(data);

    auto pathEntry = findOne([&](int32_t id, const Path& path)
    {
        return path.path.front() == start && path.path.back() == end;
    }, data.tPath);

    if(pathEntry)
    {
        return {PathEntry{pathEntry->id, pathEntry->data}};
    }
    else
    {
        Path path = pathfinder.findPath(adaptor, start, end);

        if(path.path.size() > 0)
        {
            auto added  = insert(std::move(path), data.tPath);

            return {PathEntry{added.id, added.data}};
        }
        else
            return {};
    }
}
