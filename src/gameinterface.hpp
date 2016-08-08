#pragma once
#include <string>
#include <thero/optional.hpp>
#include <glm/glm.hpp>
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>
#include "data/data.hpp"

class GameInterface
{
    public:
        struct Output
        {
            std::vector<RoomTask> roomTasks;
        };

        enum State { IDLE, PLAN_ROOM };

        GameInterface();
        void updateAndRender(fea::Renderer2D& renderer);
        void worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        Output fetchOutput();
    private:
        std::string stateToString(State state) const;
        State mState;

        th::Optional<glm::ivec2> mRoomStart;
        th::Optional<glm::ivec2> mRoomEnd;

        //out
        std::vector<RoomTask> mNewRoomTasks;
};
