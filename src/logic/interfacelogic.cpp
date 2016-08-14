#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include "../taskutil.hpp"
#include "../space.hpp"
#include <imgui.h>

InterfaceLogic::InterfaceLogic(Space& space, fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask, IdSet& unassignedTasks):
    mState(IDLE),
	mSpace(space),
    mRenderer(renderer),
    mGameSpeedMultiplier(gameSpeedMultiplier),
    mTaskIdPool(taskIdPool),
    mWalls(walls),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask),
    mUnassignedTasks(unassignedTasks),
    mShowZones(showZones),
    mShowAtmosphere(showAtmosphere)
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

    ImGui::Checkbox("Show zones", &mShowZones);
    ImGui::Checkbox("Show atmosphere", &mShowAtmosphere);

    if(ImGui::SmallButton("Create scenario"))
    {
		mSpace.startScenario();
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
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == PLAN_ROOM)
    {
        mRoomStart = mRoomEnd = tile;
    }
    //TEMP
    else
    {
        glm::ivec2 offset = button == fea::Mouse::LEFT ? glm::ivec2(16, 0) : glm::ivec2(0, 16);
        int32_t idToSet = mWalls.at((position + offset) / 32, button == fea::Mouse::LEFT ? Orientation::Vertical : Orientation::Horizontal) ? 0 : 1;
        mWalls.set((position + offset) / 32, button == fea::Mouse::LEFT ? Orientation::Vertical : Orientation::Horizontal, idToSet);
    }
    //ENDTEMP
}

void InterfaceLogic::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == PLAN_ROOM)
    {
        mRoomEnd = tile;

        mRoomStart->x = std::min(mRoomStart->x, mRoomEnd->x);
        mRoomStart->y = std::min(mRoomStart->y, mRoomEnd->y);
    }
}

void InterfaceLogic::worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == PLAN_ROOM)
    {
        addTask(mTaskIdPool.next(), RoomTask
        {
            *mRoomStart,
            *mRoomEnd - *mRoomStart + glm::ivec2(1, 1),
        }, mTRoomTask, mUnassignedTasks);

        for(int32_t x = mRoomStart->x; x <= mRoomEnd->x; ++x)
        {
            addTask(mTaskIdPool.next(), WallTask
            {
                {x, mRoomStart->y},
                Orientation::Horizontal,
            }, mTWallTask, mUnassignedTasks);
            addTask(mTaskIdPool.next(), WallTask
            {
                {x, mRoomEnd->y + 1},
                Orientation::Horizontal,
            }, mTWallTask, mUnassignedTasks);
        }

        for(int32_t y = mRoomStart->y; y <= mRoomEnd->y; ++y)
        {
            addTask(mTaskIdPool.next(), WallTask
            {
                {mRoomStart->x, y},
                Orientation::Vertical,
            }, mTWallTask, mUnassignedTasks);
            addTask(mTaskIdPool.next(), WallTask
            {
                {mRoomEnd->x + 1, y},
                Orientation::Vertical,
            }, mTWallTask, mUnassignedTasks);
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
