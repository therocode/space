#include "zonelogic.hpp"
#include "../wallpathadaptor.hpp"
#include <fea/util.hpp>
#include "../debug.hpp"

ZoneLogic::ZoneLogic(const WallMap& walls, Zones& zones):
    mWalls(walls),
    mZones(zones),
    mNextZoneId(1)
{
}

void ZoneLogic::update(const std::vector<WallChange>& changedWalls)
{
    updateZones(changedWalls);
}

void ZoneLogic::updateZones(const std::vector<WallChange>& changedWalls)
{
    auto neighbors = [&] (const glm::ivec2& node, const std::unordered_set<glm::ivec2>& ignoreNodes)
    {
        std::vector<glm::ivec2> result;

        if(!mWalls.at(node, Orientation::Horizontal) && ignoreNodes.count(node + glm::ivec2(0, -1)) == 0 && node.y > 0)
            result.push_back(node + glm::ivec2(0, -1));
        if(!mWalls.at(node, Orientation::Vertical) && ignoreNodes.count(node + glm::ivec2(-1, 0)) == 0 && node.x > 0)
            result.push_back(node + glm::ivec2(-1, 0));
        if(!mWalls.at(node + glm::ivec2(0, 1), Orientation::Horizontal) && ignoreNodes.count(node + glm::ivec2(0, 1)) == 0 && node.y < mWalls.size().y - 2)
            result.push_back(node + glm::ivec2(0, 1));
        if(!mWalls.at(node + glm::ivec2(1, 0), Orientation::Vertical) && ignoreNodes.count(node + glm::ivec2(1, 0)) == 0 && node.x < mWalls.size().x - 2)
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

            auto currentNeighbors = neighbors(current, filled);
            toFill.insert(currentNeighbors.begin(), currentNeighbors.end());

            set(current, id, mZones);
            filled.insert(std::move(current));
        }
    };

    fea::Pathfinder<WallPathAdaptor> pathfinder;
    WallPathAdaptor pathAdaptor(mWalls);

    for(const auto& changedWall : changedWalls)
    {
        glm::ivec2 start = changedWall.position;
        glm::ivec2 end = changedWall.position + (changedWall.orientation == Orientation::Vertical ? glm::ivec2(-1, 0) : glm::ivec2(0, -1));

        auto path = pathfinder.findPath(pathAdaptor, start, end);

        dbgPrintV(changedWalls.size());
        if(path.empty())
        {
            std::unordered_set<glm::ivec2> startSideQueue = {start};
            std::unordered_set<glm::ivec2> endSideQueue = {end};
            std::unordered_set<glm::ivec2> discardedStartSide;
            std::unordered_set<glm::ivec2> discardedEndSide;

            bool startIsSmaller = false;
            while(true)
            {
                glm::ivec2 startSideNode = std::move(*startSideQueue.begin()); startSideQueue.erase(startSideQueue.begin());

                auto startNeighbors = neighbors(startSideNode, discardedStartSide);

                startSideQueue.insert(startNeighbors.begin(), startNeighbors.end());
                discardedStartSide.insert(startSideNode);

                if(startSideQueue.empty())
                {
                    startIsSmaller = true;
                    break;
                }

                glm::ivec2 endSideNode = std::move(*endSideQueue.begin()); endSideQueue.erase(endSideQueue.begin());

                auto endNeighbors = neighbors(endSideNode, discardedEndSide);

                endSideQueue.insert(endNeighbors.begin(), endNeighbors.end());
                discardedEndSide.insert(endSideNode);

                if(endSideQueue.empty())
                {
                    startIsSmaller = false;
                    break;
                }
            }

            dbgPrintV(startIsSmaller);
            dbgPrintV(start);
            dbgPrintV(end);

            if(startIsSmaller)
            {
                zoneFill(start, ++mNextZoneId);
            }
            else
            {
                zoneFill(end, ++mNextZoneId);
            }
        }
    }
    //grab wall change list and operate on it. maybe grab it by diffing walls? 
    //pathfind opposite tiles of the wall. if they can't meet, new zone is made

    //lower number has priority in taking over zones
}
