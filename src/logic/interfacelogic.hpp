#pragma once
#include <string>
#include <thero/optional.hpp>
#include <glm/glm.hpp>
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>
#include <data.hpp>
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"
#include <fea/util.hpp>
#include <unordered_set>
#include "../wallposition.hpp"

class Space;

class InterfaceLogic
{
    struct RoomPlanInfo
    {
        glm::ivec2 start;
        glm::ivec2 end;
        th::Optional<glm::ivec2> initialPos;
        std::unordered_set<WallPosition> doors;
    };

    public:
        struct Output
        {
            std::vector<RoomTask> roomTasks;
        };

        enum State { IDLE, DRAGGING_ROOM, PLANNING_ROOM };

        InterfaceLogic(Space& space, fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, WallMap& walls, TaskData& tsk, EntityData& ent);
        void update();
        void worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void keyPressed(int32_t code);
    private:
        std::string stateToString(State state) const;
        void reset();
        State mState;

        th::Optional<glm::ivec2> mDragStart;
        th::Optional<glm::ivec2> mDragEnd;
        th::Optional<RoomPlanInfo> mRoomPlan;

		Space& mSpace;
        fea::Renderer2D& mRenderer;
        int32_t& mGameSpeedMultiplier;
        NumberPool<int32_t>& mTaskIdPool;
        WallMap& mWalls;
        TaskData& mTsk;
        EntityData& mEnt;
        bool& mShowZones;
        bool& mShowAtmosphere;
        fea::FrameTimer mFrameTimer;
};
