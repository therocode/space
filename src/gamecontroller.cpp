#include "gamecontroller.hpp"
#include <imgui.h>

GameController::GameController():
    mState(IDLE)
{
}

void GameController::updateAndRender(fea::Renderer2D& renderer)
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
        fea::Quad roomQuad(static_cast<glm::vec2>((*mRoomEnd) - (*mRoomStart) + glm::ivec2{1, 1}) * 32.0f);

        roomQuad.setPosition(static_cast<glm::vec2>(*mRoomStart) * 32.0f);

        renderer.render(roomQuad);
    }
}

void GameController::worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomStart = mRoomEnd = tile;
    }
}

void GameController::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomEnd = tile;

        mRoomStart->x = std::min(mRoomStart->x, mRoomEnd->x);
        mRoomStart->y = std::min(mRoomStart->y, mRoomEnd->y);
    }
}

std::string GameController::stateToString(State state) const
{
    if(state == IDLE)
        return "Idle";
    else if(state == PLAN_ROOM)
        return "Build Room";
    else
        return "None";
}
