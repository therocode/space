#pragma once
#include <string>
#include <thero/optional.hpp>
#include <glm/glm.hpp>
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>

class GameController
{
    enum State { IDLE, PLAN_ROOM };
    public:
        GameController();
        void updateAndRender(fea::Renderer2D& renderer);
        void worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
    private:
        std::string stateToString(State state) const;
        State mState;

        th::Optional<glm::ivec2> mRoomStart;
        th::Optional<glm::ivec2> mRoomEnd;
};
