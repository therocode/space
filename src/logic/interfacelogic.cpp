#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include <imgui.h>

InterfaceLogic::InterfaceLogic(fea::Renderer2D& renderer, NumberPool<int32_t>& taskIdPool, tsk::TRoomTask& tRoomTask):
    mState(IDLE),
    mRenderer(renderer),
    mTaskIdPool(taskIdPool),
    mTRoomTask(tRoomTask)
{
}

void InterfaceLogic::update()
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

        mRenderer.render(roomRect);
    }
}

void InterfaceLogic::worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomStart = mRoomEnd = tile;
    }
}

void InterfaceLogic::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        mRoomEnd = tile;

        mRoomStart->x = std::min(mRoomStart->x, mRoomEnd->x);
        mRoomStart->y = std::min(mRoomStart->y, mRoomEnd->y);
    }
}

void InterfaceLogic::worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(mState == PLAN_ROOM)
    {
        insert(mTaskIdPool.next(), RoomTask
        {
            *mRoomStart,
            *mRoomEnd - *mRoomStart + glm::ivec2(1, 1),
        }, mTRoomTask);

        mRoomStart = {};
        mRoomEnd = {};

        mState = IDLE;
    }
}

std::string InterfaceLogic::stateToString(State state) const
{
    if(state == IDLE)
        return "Idle";
    else if(state == PLAN_ROOM)
        return "Build Room";
    else
        return "None";
}
