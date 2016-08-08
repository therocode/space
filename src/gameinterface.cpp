#include "gameinterface.hpp"
#include "drawables/linerect.hpp"
#include <imgui.h>

GameInterface::GameInterface():
    mState(IDLE)
{
}

void GameInterface::updateAndRender(fea::Renderer2D& renderer)
{
    ImGui::Begin("Space");
    ImGui::Text("%s", stateToString(mState).c_str());

    if(ImGui::SmallButton("Build Room"))
    {
        mState = PLAN_ROOM;
    }

    ImGui::End();

    if(mRoomStart && mRoomEnd)
    {
        LineRect roomRect(static_cast<glm::vec2>((*mRoomEnd) - (*mRoomStart) + glm::ivec2{1, 1}) * 32.0f);

        roomRect.setColor(fea::Color::Green);
        roomRect.setPosition(static_cast<glm::vec2>(*mRoomStart) * 32.0f);

        renderer.render(roomRect);
    }
}

void GameInterface::worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomStart = mRoomEnd = tile;
    }
}

void GameInterface::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomEnd = tile;

        mRoomStart->x = std::min(mRoomStart->x, mRoomEnd->x);
        mRoomStart->y = std::min(mRoomStart->y, mRoomEnd->y);
    }
}

void GameInterface::worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mNewRoomTasks.push_back(RoomTask
        {
            *mRoomStart,
            *mRoomEnd - *mRoomStart + glm::ivec2(1, 1),
        });
        mRoomStart = {};
        mRoomEnd = {};

        mState = IDLE;
    }
}

std::string GameInterface::stateToString(State state) const
{
    if(state == IDLE)
        return "Idle";
    else if(state == PLAN_ROOM)
        return "Build Room";
    else
        return "None";
}

GameInterface::Output GameInterface::fetchOutput()
{
    GameInterface::Output output
    {
        std::move(mNewRoomTasks),
    };

    mNewRoomTasks.clear();

    return output;
}
