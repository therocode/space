#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include "../taskutil.hpp"
#include "../space.hpp"
#include <imgui.h>

InterfaceLogic::InterfaceLogic(Space& space, fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, WallMap& walls, TaskData& tsk, EntityData& ent):
    mState(IDLE),
	mSpace(space),
    mRenderer(renderer),
    mGameSpeedMultiplier(gameSpeedMultiplier),
    mTaskIdPool(taskIdPool),
    mWalls(walls),
    mTsk(tsk),
    mEnt(ent),
    mShowZones(showZones),
    mShowAtmosphere(showAtmosphere)
{
    (void)mEnt;
}

void InterfaceLogic::update()
{
    mFrameTimer.sample();
    ImGui::Begin("Space");
    ImGui::Text("%s", std::string(std::to_string(mFrameTimer.fps()) + " fps - " + std::to_string(mFrameTimer.avgFrameTime()) + " ms").c_str());
    ImGui::SliderInt("Game speed", &mGameSpeedMultiplier, 0, 16);
    ImGui::Text("%s", stateToString(mState).c_str());

    if(ImGui::SmallButton("Build Room"))
    {
        mState = DRAGGING_ROOM;
    }

    ImGui::Checkbox("Show zones", &mShowZones);
    ImGui::Checkbox("Show atmosphere", &mShowAtmosphere);

    if(ImGui::SmallButton("Create scenario"))
    {
		mSpace.startScenario();
    }

    ImGui::End();

    if(mDragStart && mDragEnd)
    {
        LineRect roomRect(static_cast<glm::vec2>((*mDragEnd) - (*mDragStart) + glm::ivec2{1, 1}) * 32.0f);

        roomRect.setColor(fea::Color::Green);
        roomRect.setPosition(static_cast<glm::vec2>(*mDragStart) * 32.0f);

        mRenderer.render(roomRect);

        if(mRoomPlan)
        {
            for(const auto& door : mRoomPlan->doors)
            {
                LineRect doorRect(door.orientation == Orientation::Horizontal ? glm::vec2(32.0f, 8.0f): glm::vec2(8.0f, 32.0f));
                doorRect.setColor(fea::Color::Green);
                doorRect.setPosition(static_cast<glm::vec2>(door.position * 32) + (door.orientation == Orientation::Horizontal ? glm::vec2(0.0f, -4.0f): glm::vec2(-4.0f, 0.0f)));

                mRenderer.render(doorRect);
            }
        }
    }

    //render room plan
    if(mRoomPlan)
    {
        if(mRoomPlan->initialPos)
        {
            ImGui::SetNextWindowPos(ImVec2(*mRoomPlan->initialPos));
            mRoomPlan->initialPos = {};
        }
        ImGui::Begin("New Room");

        ImGui::Text("%s", std::string("Position: " + std::to_string(mRoomPlan->start.x) + " " + std::to_string(mRoomPlan->start.y)).c_str());
        ImGui::Text("%s", std::string("Size: " + std::to_string(mRoomPlan->end.x - mRoomPlan->start.x) + " " + std::to_string(mRoomPlan->end.y - mRoomPlan->start.y)).c_str());
        ImGui::Text("%s", std::string("Doors: " + std::to_string(mRoomPlan->doors.size())).c_str());

        if(ImGui::SmallButton("Done"))
        {
            addTask(mTaskIdPool.next(), RoomTask
            {
                *mDragStart,
                *mDragEnd - *mDragStart + glm::ivec2(1, 1),
            }, mTsk.tRoomTask, mTsk.unassignedTasks);

            auto hasDoor = [&] (const WallPosition& pos)
            {
                return mRoomPlan->doors.count(pos) != 0;
            };

            for(int32_t x = mDragStart->x; x <= mDragEnd->x; ++x)
            {
                if(hasDoor({{x, mDragStart->y}, Orientation::Horizontal}))
                    addTask(mTaskIdPool.next(), DoorTask
                    {
                        {x, mDragStart->y},
                        Orientation::Horizontal,
                    }, mTsk.tDoorTask, mTsk.unassignedTasks);
                else
                    addTask(mTaskIdPool.next(), WallTask
                    {
                        {x, mDragStart->y},
                        Orientation::Horizontal,
                    }, mTsk.tWallTask, mTsk.unassignedTasks);
                if(hasDoor({{x, mDragStart->y + 1}, Orientation::Horizontal}))
                    addTask(mTaskIdPool.next(), WallTask
                    {
                            {x, mDragEnd->y + 1},
                            Orientation::Horizontal,
                    }, mTsk.tWallTask, mTsk.unassignedTasks);
                else
                    addTask(mTaskIdPool.next(), WallTask
                    {
                            {x, mDragEnd->y + 1},
                            Orientation::Horizontal,
                    }, mTsk.tWallTask, mTsk.unassignedTasks);
            }

            for(int32_t y = mDragStart->y; y <= mDragEnd->y; ++y)
            {
                if(hasDoor({{mDragStart->x, y}, Orientation::Vertical}))
                    addTask(mTaskIdPool.next(), DoorTask
                    {
                            {mDragEnd->x, y},
                            Orientation::Vertical,
                    }, mTsk.tDoorTask, mTsk.unassignedTasks);
                else
                    addTask(mTaskIdPool.next(), WallTask
                    {
                            {mDragStart->x, y},
                            Orientation::Vertical,
                    }, mTsk.tWallTask, mTsk.unassignedTasks);
                if(hasDoor({{mDragStart->x + 1, y}, Orientation::Vertical}))
                    addTask(mTaskIdPool.next(), DoorTask
                    {
                            {mDragEnd->x + 1, y},
                            Orientation::Vertical,
                    }, mTsk.tDoorTask, mTsk.unassignedTasks);
                else
                    addTask(mTaskIdPool.next(), WallTask
                    {
                            {mDragEnd->x + 1, y},
                            Orientation::Vertical,
                    }, mTsk.tWallTask, mTsk.unassignedTasks);
            }

            reset();
        }

        ImGui::End();
    }
}

void InterfaceLogic::worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;
    else if(button == fea::Mouse::RIGHT)
    {
        reset();
    }
    else
    {
        if(mState == DRAGGING_ROOM || (mState == PLANNING_ROOM && (!mDragStart || tile.x < mDragStart->x || tile.y < mDragStart->y || tile.x > mDragEnd->x || tile.y > mDragEnd->y)))
        {
            mDragStart = mDragEnd = tile;
            mState = DRAGGING_ROOM;
        }
        else if(mState == PLANNING_ROOM)
        {
            auto toggleDoor = [&] (const glm::ivec2& pos, Orientation orientation)
            {
                glm::ivec2 doorTile = pos / 32;
                auto iter = mRoomPlan->doors.find({doorTile, orientation});

                if(iter != mRoomPlan->doors.end())
                    mRoomPlan->doors.erase(iter);
                else
                    mRoomPlan->doors.emplace(WallPosition{doorTile, orientation});
            };

            if(tile.x >= mDragStart->x && tile.y >= mDragStart->y && tile.x <= mDragEnd->x && tile.y <= mDragEnd->y)
            {
                glm::ivec2 start = *mDragStart * 32;
                glm::ivec2 end = *mDragEnd * 32;

                if(std::abs(position.x - start.x) < 10)
                    toggleDoor(position, Orientation::Vertical);
                else if(std::abs(position.x - end.x - 32) < 10)
                    toggleDoor(position + glm::ivec2(32, 0), Orientation::Vertical);
                else if(std::abs(position.y - start.y) < 10)
                    toggleDoor(position, Orientation::Horizontal);
                else if(std::abs(position.y - end.y - 32) < 10)
                    toggleDoor(position + glm::ivec2(0, 32), Orientation::Horizontal);
            }
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
}

void InterfaceLogic::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == DRAGGING_ROOM)
    {
        if(!mDragEnd)
            mDragEnd = glm::ivec2();
        if(tile.x != mDragStart->x - 1)
            mDragEnd->x = tile.x;
        if(tile.y != mDragStart->y - 1)
            mDragEnd->y = tile.y;
    }
}

void InterfaceLogic::worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == DRAGGING_ROOM)
    {
        mState = PLANNING_ROOM;
        mRoomPlan = RoomPlanInfo{*mDragStart, *mDragEnd, *mDragStart * 32, {}};

        if(mDragStart->x > mDragEnd->x)
            std::swap(mDragStart->x, mDragEnd->x);
        if(mDragStart->y > mDragEnd->y)
            std::swap(mDragStart->y, mDragEnd->y);
    }
}

std::string InterfaceLogic::stateToString(State state) const
{
    if(state == IDLE)
        return "Idle";
    else if(state == DRAGGING_ROOM)
        return "Drag Room";
    else if(state == PLANNING_ROOM)
        return "Plan Room";
    else
        return "None";
}

void InterfaceLogic::reset()
{
    mState = IDLE;
    mDragStart = {};
    mDragEnd = {};
    mRoomPlan = {};
}
