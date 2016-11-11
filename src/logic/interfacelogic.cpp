#include "interfacelogic.hpp"
#include "../drawables/linerect.hpp"
#include "../taskutil.hpp"
#include "../space.hpp"
#include "../structuregui.hpp"
#include <imgui.h>
#include "../imguiutil.hpp"
#include "../roomutil.hpp"
#include "../doorutil.hpp"
#include "../wallutil.hpp"
#include "../atmosphereutil.hpp"

InterfaceLogic::InterfaceLogic(Space& space, fea::Renderer2D& renderer, ResourceManager& resources, int32_t& gameSpeedMultiplier, int32_t& stepAmount, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, GameData& data):
    mState(IDLE),
    mPaintAtmosphereInfo({cHealthyAtmosphere}),
	mSpace(space),
    mRenderer(renderer),
    mResources(resources),
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
    if(ImGui::SmallButton("Convert to Door"))
    {
        mState = PLACING_DOORS;
        mDoorsPlan = {{}};
    }
    if(ImGui::SmallButton("Build structure"))
    {
        mState = BUILD_STRUCTURE;
        mBuildStructureInfo = {{}};
    }

    ImGui::Checkbox("Show zones", &mShowZones);
    ImGui::Checkbox("Show atmosphere", &mShowAtmosphere);

    if(ImGui::SmallButton("Create scenario"))
    {
		mSpace.startScenario();
    }

    if(ImGui::SmallButton("Paint atmosphere"))
    {
        mState = PAINT_ATMOSPHERE;
    }

    if(ImGui::SmallButton("Edit walls/doors"))
    {
        mState = EDIT_WALLS_DOORS;
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

    //render door plan
    if(mDoorsPlan)
    {
        for(const auto& door : mDoorsPlan->doors)
        {
            LineRect doorRect(door.orientation == Orientation::Horizontal ? glm::vec2(32.0f, 8.0f): glm::vec2(8.0f, 32.0f));
            doorRect.setColor(fea::Color::Green);
            doorRect.setPosition(static_cast<glm::vec2>(door.position * 32) + (door.orientation == Orientation::Horizontal ? glm::vec2(0.0f, -4.0f): glm::vec2(-4.0f, 0.0f)));

            mRenderer.render(doorRect);
        }

        if(mDoorsPlan->initialPos)
        {
            ImGui::SetNextWindowPos(ImVec2(*mDoorsPlan->initialPos));
            mDoorsPlan->initialPos = {};
        }
        ImGui::Begin("Convert walls to doors");
        ImGui::Text("click the walls to add doors");

        ImGui::Text("%s", std::string("Doors: " + std::to_string(mDoorsPlan->doors.size())).c_str());

        bool hasAtLeastOneDoor = !mDoorsPlan->doors.empty();

        if(SmallDisableButton("Done", !hasAtLeastOneDoor))
        {
            for(const auto& door : mDoorsPlan->doors)
            {
                addTask(DoorTask
                {
                    door,
                }, mData.tDoorTask, mData);
            }

            reset();
        }
        ImGui::End();
    }

    //render build plan
    if(mBuildStructureInfo)
    {
        for(const auto& plan : mBuildStructureInfo->plans)
        {
            fea::Quad structureQuad({32.0f, 32.0f});
            structureQuad.setPosition(plan.first * glm::ivec2(32, 32));
            structureQuad.setOpacity(0.4f);
            const fea::Texture& texture = *mResources.textures().at(static_cast<size_t>(get(plan.second, mData.tStructureType).texture)).texture;
            structureQuad.setTexture(texture);

            mRenderer.render(structureQuad);
        }

        ImGui::Begin("Build structure");
        ImGui::Text("Select and place structures you want to build");

        forEach([&](int32_t id, const StructureType& type)
        {
            if(!mBuildStructureInfo->selectedStructureType)
                mBuildStructureInfo->selectedStructureType = type.type;

            ImColor buttonColor(0, 0, 0, 255);

            if(type.type == mBuildStructureInfo->selectedStructureType)
                buttonColor = {100, 150, 200, 255};

            if(ImGui::ImageButton(reinterpret_cast<void*>(mResources.textures().at(static_cast<size_t>(type.texture)).texture->getId()), {32, 32}, {0.0, 0.0}, {1.0, 1.0f}, -1, buttonColor))
            {
                mBuildStructureInfo->selectedStructureType = type.type;
            }

            if(ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s",type.name.c_str());
                ImGui::EndTooltip();
            }
        }, mData.tStructureType);

        if(ImGui::SmallButton("Done"))
        {

            for(const auto& structurePlan : mBuildStructureInfo->plans)
            {
                addTask(StructureTask
                {
                    structurePlan.second,
                    structurePlan.first
                }, mData.tStructureTask, mData);
            }

            reset();
        }

        ImGui::End();
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

    //render atmosphere paint
    if(mState == PAINT_ATMOSPHERE)
    {
        ImGui::Begin("Atmosphere values");
        ImGui::Text("Drag sliders to change what composition you paint");

        ImGui::SliderInt("Oxygen", &mPaintAtmosphereInfo.atmosphereColor[Oxygen], 0, 500000, NULL);
        ImGui::SliderInt("Carbon Dioxide", &mPaintAtmosphereInfo.atmosphereColor[CarbonDioxide], 0, 500000, NULL);
        ImGui::SliderInt("Nitrogen", &mPaintAtmosphereInfo.atmosphereColor[Nitrogen], 0, 500000, NULL);
        ImGui::Text("Pressure: %d%%", static_cast<int32_t>(static_cast<float>(pressure(mPaintAtmosphereInfo.atmosphereColor))/pressure(cHealthyAtmosphere) * 100.0f));

        if(ImGui::SmallButton("Healthy Atmosphere"))
        {
            mPaintAtmosphereInfo.atmosphereColor = cHealthyAtmosphere;
        }
        if(ImGui::SmallButton("Ambient Atmosphere"))
        {
            mPaintAtmosphereInfo.atmosphereColor = mData.defaultAtmosphere;
        }
        if(ImGui::SmallButton("Vacuum"))
        {
            mPaintAtmosphereInfo.atmosphereColor = {};
        }

        ImGui::End();
    }

    //render wall door box
    if(mState == EDIT_WALLS_DOORS)
    {
        ImGui::Begin("Edit walls and doors");

        if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Walls)
            ImGui::Text("Click/drag to toggle walls");
        if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Doors)
            ImGui::Text("Click/drag to toggle doors on walls");
        if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Lock)
            ImGui::Text("Click/drag to lock/unlock doors");

        if(ImGui::SmallButton("Edit walls"))
            mEditWallsDoorsInfo.placeState = EditWallsDoorsInfo::Walls;
        if(ImGui::SmallButton("Edit doors"))
            mEditWallsDoorsInfo.placeState = EditWallsDoorsInfo::Doors;
        if(ImGui::SmallButton("Edit locking"))
            mEditWallsDoorsInfo.placeState = EditWallsDoorsInfo::Lock;

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
    {
        return;
    }
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
        else if(mState == BUILD_STRUCTURE)
        {
            if(mBuildStructureInfo->plans.count(tile) == 0)
                mBuildStructureInfo->plans[tile] = *mBuildStructureInfo->selectedStructureType;
            else
                mBuildStructureInfo->plans.erase(tile);
        }
        else if(mState == PLACING_DOORS)
        {
            auto clickedWall = positionToWall(position, 10.0f);

            if(clickedWall && hasWall(*clickedWall, mData) && !hasDoorTask(*clickedWall, mData))
            {
                if(mDoorsPlan->doors.count(*clickedWall) == 0)
                    mDoorsPlan->doors.emplace(*clickedWall);
                else
                    mDoorsPlan->doors.erase(*clickedWall);
            }
        }
        else if(mState == PAINT_ATMOSPHERE)
        {
            setAtmosphere(tile, mPaintAtmosphereInfo.atmosphereColor, mData);
        }
        else if(mState == EDIT_WALLS_DOORS)
        {
            auto maybeWall = positionToWall(position, 10.0f);

            if(maybeWall)
            {
                WallPosition wallPosition = *maybeWall;

                if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Walls)
                {
                    if(!hasWall(wallPosition, mData))
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::On;
                    else
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::Off;
                }
                else if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Doors)
                {
                    if(hasDoor(wallPosition, mData))
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::Off;
                    else
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::On;
                }
                else if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Lock)
                {
                    if(lockedDoorAt(wallPosition, mData))
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::Off;
                    else
                        mEditWallsDoorsInfo.toggleState = EditWallsDoorsInfo::On;
                }

                applyEditWallsDoors(wallPosition);
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
    else if(mState == PAINT_ATMOSPHERE)
    {
        setAtmosphere(tile, mPaintAtmosphereInfo.atmosphereColor, mData);
    }
    else if(mState == EDIT_WALLS_DOORS)
    {
        auto maybeWall = positionToWall(position, 10.0f);
        if(maybeWall)
        {
            WallPosition wallPosition = *maybeWall;

            applyEditWallsDoors(wallPosition);
        }
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
    else if(state == BUILD_STRUCTURE)
        return "Build Structure";
    else if(state == PLACING_DOORS)
        return "Place Doors On Walls";
    else if(state == PAINT_ATMOSPHERE)
        return "Paint Atmosphere";
    else
        return "None";
}

void InterfaceLogic::reset()
{
    mState = IDLE;
    mDragStart = {};
    mDragEnd = {};
    mRoomPlan = {};
    mDoorsPlan = {};
    mStructureInteraction = {};
    mBuildStructureInfo = {};
}

void InterfaceLogic::applyEditWallsDoors(const WallPosition& position)
{
    if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Walls)
    {
        if(mEditWallsDoorsInfo.toggleState == EditWallsDoorsInfo::On)
            setWall(position, 1, mData);
        else
            setWall(position, 0, mData);
    }
    else if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Doors)
    {
        if(mEditWallsDoorsInfo.toggleState == EditWallsDoorsInfo::On && hasWall(position, mData))
            createDoor(Door{position}, mData);
        else if(mEditWallsDoorsInfo.toggleState == EditWallsDoorsInfo::Off && hasDoor(position, mData))
            removeDoorAt(position, mData);
    }
    else if(mEditWallsDoorsInfo.placeState == EditWallsDoorsInfo::Lock)
    {
        auto door = doorAt(position, mData);

        if(door)
        {
            if(mEditWallsDoorsInfo.toggleState == EditWallsDoorsInfo::On)
                lockDoor(*door, mData);
            else
                unlockDoor(*door, mData);
        }
    }
}
