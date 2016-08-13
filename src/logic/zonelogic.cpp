#include "zonelogic.hpp"
#include "../wallpathadaptor.hpp"
#include <fea/util.hpp>
#include "../debug.hpp"

ZoneLogic::ZoneLogic(const WallMap& walls, Zones& zones):
    mWalls(walls),
    mZones(zones)
{
    mZoneIds.next(); //occupy 0 as it will never be free
}

void ZoneLogic::update(const std::vector<WallChange>& changedWalls)
{
    updateZones(changedWalls);
}

void ZoneLogic::updateZones(const std::vector<WallChange>& changedWalls)
{
    auto neighbors = [&] (const glm::ivec2& node, const std::unordered_set<glm::ivec2>& ignoreNodes, int32_t ignoreId)
    {
        std::vector<glm::ivec2> result;

        if(!mWalls.at(node, Orientation::Horizontal) && ignoreNodes.count(node + glm::ivec2(0, -1)) == 0 && at(node + glm::ivec2(0, -1), mZones) != ignoreId && node.y > 0)
            result.push_back(node + glm::ivec2(0, -1));
        if(!mWalls.at(node, Orientation::Vertical) && ignoreNodes.count(node + glm::ivec2(-1, 0)) == 0 && at(node + glm::ivec2(-1, 0), mZones) != ignoreId && node.x > 0)
            result.push_back(node + glm::ivec2(-1, 0));
        if(!mWalls.at(node + glm::ivec2(0, 1), Orientation::Horizontal) && ignoreNodes.count(node + glm::ivec2(0, 1)) == 0 && at(node + glm::ivec2(0, 1), mZones) != ignoreId && node.y < mWalls.size().y - 2)
            result.push_back(node + glm::ivec2(0, 1));
        if(!mWalls.at(node + glm::ivec2(1, 0), Orientation::Vertical) && ignoreNodes.count(node + glm::ivec2(1, 0)) == 0 && at(node + glm::ivec2(1, 0), mZones) != ignoreId && node.x < mWalls.size().x - 2)
            result.push_back(node + glm::ivec2(1, 0));

        return result;
    };

    auto zoneFill = [&] (const glm::ivec2& start, int32_t id)
    {
        std::unordered_set<glm::ivec2> toFill{start};
        std::unordered_set<glm::ivec2> filled;

        while(!toFill.empty())
        {
            glm::ivec2 current = std::move(*toFill.begin()); toFill.erase(toFill.begin());

            auto currentNeighbors = neighbors(current, filled, id);
            toFill.insert(currentNeighbors.begin(), currentNeighbors.end());

            set(current, id, mZones);
            filled.insert(std::move(current));
        }
    };

    fea::Pathfinder<WallPathAdaptor> pathfinder;
    WallPathAdaptor pathAdaptor(mWalls);
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
        glm::ivec2 start = changedWall.position;
        glm::ivec2 end = changedWall.position + (changedWall.orientation == Orientation::Vertical ? glm::ivec2(-1, 0) : glm::ivec2(0, -1));
        int32_t startId = at(start, mZones);
        int32_t endId = at(end, mZones);

        if(startId == endId)
        {
            if(changedWall.id == 0) //skip early if the change in wall type can't have any effect
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
                    zoneFill(start, mZoneIds.next());
                }
                else
                {
                    zoneFill(end, mZoneIds.next());
                }
            }
        }
        else
        {
            if(changedWall.id != 0) //skip early if the change in wall type can't have any effect
                continue;
            
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
    //grab wall change list and operate on it. maybe grab it by diffing walls? 
    //pathfind opposite tiles of the wall. if they can't meet, new zone is made

    //lower number has priority in taking over zones
}