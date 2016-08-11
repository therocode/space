#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include <imgui.h>

InterfaceLogic::InterfaceLogic(fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, NumberPool<int32_t>& taskIdPool, WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask):
    mState(IDLE),
    mRenderer(renderer),
    mGameSpeedMultiplier(gameSpeedMultiplier),
    mTaskIdPool(taskIdPool),
    mWalls(walls),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask)
{
}

void InterfaceLogic::update()
{
    ImGui::Begin("Space");
    ImGui::SliderInt("Game speed", &mGameSpeedMultiplier, 0, 16);
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
    //TEMP
    else
    {
        mWalls.set(position / 32, button == fea::Mouse::LEFT ? Orientation::Vertical : Orientation::Horizontal, 1);
    }
    //ENDTEMP
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

        for(int32_t x = mRoomStart->x; x <= mRoomEnd->x; ++x)
        {
            insert(mTaskIdPool.next(), WallTask
            {
                {x, mRoomStart->y},
                Orientation::Horizontal,
            }, mTWallTask);
            insert(mTaskIdPool.next(), WallTask
            {
                {x, mRoomEnd->y + 1},
                Orientation::Horizontal,
            }, mTWallTask);
        }

        for(int32_t y = mRoomStart->y; y <= mRoomEnd->y; ++y)
        {
            insert(mTaskIdPool.next(), WallTask
            {
                {mRoomStart->x, y},
                Orientation::Vertical,
            }, mTWallTask);
            insert(mTaskIdPool.next(), WallTask
            {
                {mRoomEnd->x + 1, y},
                Orientation::Vertical,
            }, mTWallTask);
        }

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
