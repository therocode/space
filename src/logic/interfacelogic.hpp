#pragma once
#include <string>
#include <thero/optional.hpp>
#include <glm/glm.hpp>
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>
#include <data.hpp>
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class InterfaceLogic
{
    public:
        struct Output
        {
            std::vector<RoomTask> roomTasks;
        };

        enum State { IDLE, PLAN_ROOM };

        InterfaceLogic(fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask, IdSet& unassignedTasks);
        void update();
        void worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
    private:
        std::string stateToString(State state) const;
        State mState;

        th::Optional<glm::ivec2> mRoomStart;
        th::Optional<glm::ivec2> mRoomEnd;

        fea::Renderer2D& mRenderer;
        int32_t& mGameSpeedMultiplier;
        NumberPool<int32_t>& mTaskIdPool;
        WallMap& mWalls;
        tsk::TRoomTask& mTRoomTask;
        tsk::TWallTask& mTWallTask;
        IdSet& mUnassignedTasks;
        bool& mShowZones;
        bool& mShowAtmosphere;
};
