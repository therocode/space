#include "zonelogic.hpp"
#include "../wallpathadaptor.hpp"
#include <fea/util.hpp>
#include "../debug.hpp"

ZoneLogic::ZoneLogic(GameData& data):
    mData(data)
{
    mZoneIds.next(); //occupy 0 as it will never be free
}

void ZoneLogic::update(WallMap& walls, const WallChanges& changedWalls)
{
    updateZones(walls, changedWalls);
}

void ZoneLogic::updateZones(WallMap& walls, const WallChanges& changedWalls)
{
    auto neighbors = [&] (const glm::ivec2& node, const std::unordered_set<glm::ivec2>& ignoreNodes, int32_t ignoreId)
    {
        std::vector<glm::ivec2> result;

        if(node.y > 0 &&                   !walls.atH(node) &&                   ignoreNodes.count(node + glm::ivec2(0, -1)) == 0 && at(node + glm::ivec2(0, -1), mData.zones) != ignoreId)
            result.push_back(node + glm::ivec2(0, -1));
        if(node.x > 0 &&                   !walls.atV(node) &&                   ignoreNodes.count(node + glm::ivec2(-1, 0)) == 0 && at(node + glm::ivec2(-1, 0), mData.zones) != ignoreId)
            result.push_back(node + glm::ivec2(-1, 0));
        if(node.y < walls.size().y - 2 && !walls.atH(node + glm::ivec2(0, 1)) && ignoreNodes.count(node + glm::ivec2(0, 1)) == 0 && at(node + glm::ivec2(0, 1), mData.zones) != ignoreId)
            result.push_back(node + glm::ivec2(0, 1));
        if(node.x < walls.size().x - 2 && !walls.atV(node + glm::ivec2(1, 0)) && ignoreNodes.count(node + glm::ivec2(1, 0)) == 0 && at(node + glm::ivec2(1, 0), mData.zones) != ignoreId)
            result.push_back(node + glm::ivec2(1, 0));

        return result;
    };

    auto zoneFill = [&] (const glm::ivec2& start, int32_t id)
    {
        int32_t oldId = mData.zones.zones.at(start);
        std::unordered_set<glm::ivec2> toFill{start};
        std::unordered_set<glm::ivec2> filled;

        while(!toFill.empty())
        {
            glm::ivec2 current = std::move(*toFill.begin());
            toFill.erase(toFill.begin());

            auto currentNeighbors = neighbors(current, filled, id);
            toFill.insert(currentNeighbors.begin(), currentNeighbors.end());

            set(current, id, mData.zones);
            filled.insert(std::move(current));
        }
    };

    fea::Pathfinder<WallPathAdaptor> pathfinder;
    WallPathAdaptor pathAdaptor(walls);
    auto tryPath = [&] (const glm::ivec2& start, const glm::ivec2& end, int32_t limit)
    {
        auto path = pathfinder.findPath(pathAdaptor, start, end, static_cast<uint32_t>(limit));

        if(path.cost != -1 && path.path.empty())
        {
            path = pathfinder.findPath(pathAdaptor, end, start, static_cast<uint32_t>(limit));
        }
        return path;
    };

    for(const auto& changedWall : changedWalls)
    {
        walls.set(changedWall.first, changedWall.second.newValue);
        glm::ivec2 start = changedWall.first.position;
        glm::ivec2 end = changedWall.first.position + (changedWall.first.orientation == Orientation::Vertical ? glm::ivec2(-1, 0) : glm::ivec2(0, -1));
        int32_t startId = at(start, mData.zones);
        int32_t endId = at(end, mData.zones);

        if(startId == endId)
        {
            if(changedWall.second.newValue == 0) //skip early if the change in wall type can't have any effect
                continue;

            auto path = tryPath(start, end, 5);
            if(path.cost != -1 && path.path.empty())
            {
                path = tryPath(start, end, 10);
            }
            if(path.cost != -1 && path.path.empty())
            {
                path = tryPath(start, end, 20);
            }
            if(path.cost != -1 && path.path.empty())
            {
                path = tryPath(start, end, -1);
            }

            if(path.path.empty())
            {
                std::unordered_set<glm::ivec2> startSideQueue = {start};
                std::unordered_set<glm::ivec2> endSideQueue = {end};
                std::unordered_set<glm::ivec2> discardedStartSide;
                std::unordered_set<glm::ivec2> discardedEndSide;

                bool startIsSmaller = false;
                while(true)
                {
                    glm::ivec2 startSideNode = std::move(*startSideQueue.begin()); startSideQueue.erase(startSideQueue.begin());

                    auto startNeighbors = neighbors(startSideNode, discardedStartSide, -1);

                    startSideQueue.insert(startNeighbors.begin(), startNeighbors.end());
                    discardedStartSide.insert(startSideNode);

                    if(startSideQueue.empty())
                    {
                        startIsSmaller = true;
                        break;
                    }

                    glm::ivec2 endSideNode = std::move(*endSideQueue.begin()); endSideQueue.erase(endSideQueue.begin());

                    auto endNeighbors = neighbors(endSideNode, discardedEndSide, -1);

                    endSideQueue.insert(endNeighbors.begin(), endNeighbors.end());
                    discardedEndSide.insert(endSideNode);

                    if(endSideQueue.empty())
                    {
                        startIsSmaller = false;
                        break;
                    }
                }

                if(startIsSmaller)
                {
                    auto id = mZoneIds.next();
                    zoneFill(start, id);
                }
                else
                {
                    auto id = mZoneIds.next();
                    zoneFill(end, id);
                }
            }
        }
        else
        {
            if(changedWall.second.newValue != 0) //skip early if the change in wall type can't have any effect
                continue;

            auto old = mData.zones.zones;
            
            if(startId < endId)
            {
                zoneFill(end, startId);
                mZoneIds.release(endId);
            }
            else
            {
                zoneFill(start, endId);
                mZoneIds.release(startId);
            }

        }
    }
}
