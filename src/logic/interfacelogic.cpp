#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include "../taskutil.hpp"
#include "../space.hpp"
#include "../structuregui.hpp"
#include <imgui.h>
#include "../imguiutil.hpp"
#include "../roomutil.hpp"
#include "../doorutil.hpp"

InterfaceLogic::InterfaceLogic(Space& space, fea::Renderer2D& renderer, int32_t& gameSpeedMultiplier, int32_t& stepAmount, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, GameData& data):
    mState(IDLE),
	mSpace(space),
    mRenderer(renderer),
    mGameSpeedMultiplier(gameSpeedMultiplier),
    mStepAmount(stepAmount),
    mData(data),
    mShowZones(showZones),
    mShowAtmosphere(showAtmosphere)
{
}

void InterfaceLogic::update()
{
    mFrameTimer.sample();
    ImGui::Begin("Space");
    ImGui::Text("%s", std::string(std::to_string(mFrameTimer.fps()) + " fps - " + std::to_string(mFrameTimer.avgFrameTime()) + " ms").c_str());
    ImGui::SliderInt("Game speed", &mGameSpeedMultiplier, 0, 16);
    if(mGameSpeedMultiplier == 0)
    {
        if(ImGui::SmallButton("Step"))
        {
            mStepAmount = 1;
        }
    }
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
        auto dragStart = *mDragStart;
        auto dragEnd = *mDragEnd;

        if(dragStart.x > dragEnd.x)
            std::swap(dragStart.x, dragEnd.x);
        if(dragStart.y > dragEnd.y)
            std::swap(dragStart.y, dragEnd.y);

        LineRect roomRect(static_cast<glm::vec2>(dragEnd - dragStart + glm::ivec2{1, 1}) * 32.0f);

        roomRect.setColor(fea::Color::Green);
        roomRect.setPosition(static_cast<glm::vec2>(dragStart) * 32.0f);

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
        ImGui::Text("click the walls to add/remove doors");

        ImGui::Text("%s", std::string("Position: " + std::to_string(mRoomPlan->start.x) + " " + std::to_string(mRoomPlan->start.y)).c_str());
        ImGui::Text("%s", std::string("Size: " + std::to_string(mRoomPlan->end.x - mRoomPlan->start.x) + " " + std::to_string(mRoomPlan->end.y - mRoomPlan->start.y)).c_str());
        ImGui::Text("%s", std::string("Doors: " + std::to_string(mRoomPlan->doors.size())).c_str());

        bool hasAtLeastOneDoor = !mRoomPlan->doors.empty();

        if(SmallDisableButton("Done", !hasAtLeastOneDoor))
        {
            //addTask(RoomTask
            //{
            //    *mDragStart,
            //    *mDragEnd - *mDragStart + glm::ivec2(1, 1),
            //}, mData.tRoomTask, mData);

            auto planHasDoor = [&] (const WallPosition& pos)
            {
                return mRoomPlan->doors.count(pos) != 0;
            };

            for(int32_t x = mDragStart->x; x <= mDragEnd->x; ++x)
            {
                WallPosition taskPos = {{x, mDragStart->y}, Orientation::Horizontal};
                if(!hasDoorTask(taskPos, mData) && !hasWallTask(taskPos, mData))
                {
                    if(planHasDoor(taskPos))
                        addTask(DoorTask
                        {
                            taskPos,
                        }, mData.tDoorTask, mData);
                    else
                        addTask(WallTask
                        {
                            taskPos,
                        }, mData.tWallTask, mData);
                }
                taskPos = {{x, mDragEnd->y + 1}, Orientation::Horizontal};
                if(!hasDoorTask(taskPos, mData) && !hasWallTask(taskPos, mData))
                {
                    if(planHasDoor(taskPos))
                        addTask(DoorTask
                        {
                            taskPos,
                        }, mData.tDoorTask, mData);
                    else
                        addTask(WallTask
                        {
                            taskPos
                        }, mData.tWallTask, mData);
                }
            }

            for(int32_t y = mDragStart->y; y <= mDragEnd->y; ++y)
            {
                WallPosition taskPos = {{mDragStart->x, y}, Orientation::Vertical};
                if(!hasDoorTask(taskPos, mData) && !hasWallTask(taskPos, mData))
                {
                    if(planHasDoor(taskPos))
                        addTask(DoorTask
                        {
                            taskPos
                        }, mData.tDoorTask, mData);
                    else
                        addTask(WallTask
                        {
                            taskPos
                        }, mData.tWallTask, mData);
                }
                taskPos = {{mDragEnd->x + 1, y}, Orientation::Vertical};
                if(!hasDoorTask(taskPos, mData) && !hasWallTask(taskPos, mData))
                {
                    if(planHasDoor(taskPos))
                        addTask(DoorTask
                        {
                            taskPos
                        }, mData.tDoorTask, mData);
                    else
                        addTask(WallTask
                        {
                            taskPos
                        }, mData.tWallTask, mData);
                }
            }

            reset();
        }
        if(!hasAtLeastOneDoor)
        {
            ImGui::SameLine();
            ImGui::Text("- add at least one door");
        }
        if(ImGui::SmallButton("Cancel"))
        {
            reset();
        }

        ImGui::End();
    }
    if(mStructureInteraction)
    {
        if(mStructureInteraction->initialPos)
        {
            ImGui::SetNextWindowPos(ImVec2(*mStructureInteraction->initialPos));
            mStructureInteraction->initialPos = {};
        }

        if(showStructureGui(mStructureInteraction->structureId, mStructureInteraction->structureTypeId, mData))
        {//close it
            reset();
        }
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
        if(mState == DRAGGING_ROOM || (mState == PLANNING_ROOM && (!mDragStart || tile.x + 1 < mDragStart->x || tile.y + 1 < mDragStart->y || tile.x - 1 > mDragEnd->x || tile.y - 1 > mDragEnd->y)))
        {
            mDragStart = mDragEnd = tile;
            mState = DRAGGING_ROOM;
        }
        else if(mState == PLANNING_ROOM)
        {
            auto toggleDoor = [&] (const WallPosition& doorPosition)
            {
                if(mData.walls.at(doorPosition) != 0 || hasWallTask(doorPosition, mData) || hasDoorTask(doorPosition, mData) || hasDoor(doorPosition, mData))
                    return;

                auto iter = mRoomPlan->doors.find(doorPosition);

                if(iter != mRoomPlan->doors.end())
                    mRoomPlan->doors.erase(iter);
                else
                    mRoomPlan->doors.emplace(doorPosition);
            };

            if(tile.x + 1 >= mDragStart->x && tile.y + 1 >= mDragStart->y && tile.x - 1 <= mDragEnd->x && tile.y - 1 <= mDragEnd->y)
            {
                glm::ivec2 start = *mDragStart * 32;
                glm::ivec2 end = *mDragEnd * 32;

                th::Optional<WallPosition> doorPos;

                if(std::abs(position.x - start.x) < 10)
                    doorPos = WallPosition{(position + glm::ivec2(16, 0)) / 32, Orientation::Vertical};
                else if(std::abs(position.x - end.x - 32) < 10)
                    doorPos = WallPosition{(position + glm::ivec2(16, 0)) / 32, Orientation::Vertical};
                else if(std::abs(position.y - start.y) < 10)
                    doorPos = WallPosition{(position + glm::ivec2(0, 16)) / 32, Orientation::Horizontal};
                else if(std::abs(position.y - end.y - 32) < 10)
                    doorPos = WallPosition{(position + glm::ivec2(0, 16)) / 32, Orientation::Horizontal};

                if(doorPos)
                {
                    if(doorPos->position.x >= mDragStart->x && doorPos->position.y >= mDragStart->y)
                    {
                        if((doorPos->orientation == Orientation::Horizontal && doorPos->position.x <= mDragEnd->x) || (doorPos->orientation == Orientation::Vertical && doorPos->position.y <= mDragEnd->y))
                            toggleDoor(*doorPos);
                    }
                }
            }
        }
        else if(mState == IDLE || mState == INTERACT_STRUCTURE)
        {
            mState = IDLE;
            mStructureInteraction = {};

            auto structureAtClick = findOne([&](int32_t id, const Structure& structure)
            {
                return structure.position == tile;
            }, mData.tStructure);

            if(structureAtClick)
            {
                mStructureInteraction = StructureInteraction
                {
                    structureAtClick->id,
                    structureAtClick->data.type,
                    {position},
                };
            }
        }
    }
}

void InterfaceLogic::worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == DRAGGING_ROOM && mDragStart)
    {
        mDragStart = tile;
    }
}

void InterfaceLogic::worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button)
{
    if(button == fea::Mouse::MIDDLE)
        return;

    if(mState == DRAGGING_ROOM)
    {
        if(mDragStart->x > mDragEnd->x)
            std::swap(mDragStart->x, mDragEnd->x);
        if(mDragStart->y > mDragEnd->y)
            std::swap(mDragStart->y, mDragEnd->y);

        mRoomPlan = RoomPlanInfo{*mDragStart, *mDragEnd, *mDragEnd * 32 + glm::ivec2(32, 32), {}};

        mState = PLANNING_ROOM;

        forEachWall(*mDragStart, *mDragEnd - *mDragStart + glm::ivec2(1, 1), [&] (const WallPosition& wallPosition)
        {
            if(hasDoor(wallPosition, mData) || hasDoorTask(wallPosition, mData))
            {
                mRoomPlan->doors.emplace(wallPosition);
            }
        });
    }
}

void InterfaceLogic::keyPressed(int32_t code)
{
    if(mState == IDLE)
    {
        if(code == fea::Keyboard::B)
            mState = DRAGGING_ROOM;
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
    else if(state == INTERACT_STRUCTURE)
        return "Structure Interaction";
    else
        return "None";
}

void InterfaceLogic::reset()
{
    mState = IDLE;
    mDragStart = {};
    mDragEnd = {};
    mRoomPlan = {};
    mStructureInteraction = {};
}
