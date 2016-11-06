#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "debugguidata.hpp"
#include "debuggui.hpp"
#include "debug.hpp"
#include "roomutil.hpp"
#include "doorutil.hpp"
#include "itemutil.hpp"
#include "gridneighbors.hpp"
#include <imgui.h>
#include "structuretypes.hpp"
#include "pathfindingutil.hpp"
#include "atmosphereutil.hpp"
#include "worldutil.hpp"
#include "wallutil.hpp"

const glm::ivec2 cMapSize(256, 256);

#ifdef EMSCRIPTEN
const fea::ContextSettings::Type contextType = fea::ContextSettings::Type::ES;
#else
const fea::ContextSettings::Type contextType = fea::ContextSettings::Type::CORE;
#endif

Space::Space() :
    mWindowSize(1366, 768),
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(static_cast<uint32_t>(mWindowSize.x), static_cast<uint32_t>(mWindowSize.y)), "Space", fea::Style::Default, fea::ContextSettings(0, 0, 0, 2, 0, contextType)),
    mFeaRenderer(fea::Viewport(mWindowSize, {0, 0}, fea::Camera(static_cast<glm::vec2>(mWindowSize / 2)))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInstantiator(mResources),
    mInputHandler(mBus, mFeaInputHandler),
    mGameSpeedMultiplier(1),
    mStepAmount(0),
    mShowZones(false),
    mShowAtmosphere(false),
    mGuiBlocksMouse(false),
    mActorLogic(mData),
    mOrganismLogic(mData),
    mDecisionAILogic(mData),
    mActionLogic(mData),
    mStructureLogic(mData, mResources),
    mTaskLogic(mData),
    mZoneLogic(mData),
    mAtmosphereLogic(mData),
    mRenderLogic(mResources, mFeaRenderer, mData, mShowZones, mShowAtmosphere),
    mInterfaceLogic(*this, mFeaRenderer, mGameSpeedMultiplier, mStepAmount, mShowZones, mShowAtmosphere, mTaskIdPool, mData)
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

    mData.walls = {cMapSize};
    mData.atmosphere = {cMapSize, mData.defaultAtmosphere};
    init(cMapSize, 0, mData.zones);

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = mWindowSize.x;
    io.DisplaySize.y = mWindowSize.y;
    io.IniFilename = "data/imgui.ini";
    io.MousePos = {0, 0};
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    mImguiFontTexture.create({width, height}, pixels);
    io.Fonts->TexID = reinterpret_cast<void*>(mImguiFontTexture.getId());

    mData.atmosphereNeighbors = findAllNeighbors(mData.atmosphere, mData.walls);
}

void Space::handleMessage(const QuitMessage& message)
{
    (void)message;
    quit();
}

void Space::handleMessage(const KeyPressedMessage& message)
{
    if(message.key == fea::Keyboard::TILDE)
        mGameSpeedMultiplier = 0;
    if(message.key == fea::Keyboard::NUM1)
        mGameSpeedMultiplier = 1;
    if(message.key == fea::Keyboard::NUM2)
        mGameSpeedMultiplier = 2;
    if(message.key == fea::Keyboard::NUM3)
        mGameSpeedMultiplier = 4;
    if(message.key == fea::Keyboard::NUM4)
        mGameSpeedMultiplier = 8;
    if(message.key == fea::Keyboard::TAB)
    {
        ++mStepAmount;
    }
    mInterfaceLogic.keyPressed(message.key);
}

void Space::handleMessage(const ResizeMessage& message)
{
    mWindowSize = message.size;
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = mWindowSize.x;
    io.DisplaySize.y = mWindowSize.y;
    mFeaRenderer.setViewport(fea::Viewport(mWindowSize, {0, 0}, fea::Camera(static_cast<glm::vec2>(mWindowSize / 2))));
}

void Space::handleMessage(const MouseClickMessage& message)
{
    if(!mGuiBlocksMouse)
    {
        mInterfaceLogic.worldMouseClick(message.position, message.position / 32, message.button);

        if(message.button == fea::Mouse::MIDDLE)
        {   
            auto actor = mInstantiator.instantiate("engineer", message.position);
            int32_t added = mActorLogic.addActor(std::move(actor));
        }   
        else if(message.button == fea::Mouse::RIGHT)
        {
            if(count(mData.tPosition))
            {
                //int32_t toDelete = rand() % count(mTPosition);
                //if(has(toDelete, mTPosition))
                //{
                //    mActorLogic.removeActor(toDelete);
                //    mActorIdPool.release(toDelete);
                //}
                //mPositions[0].position = message.position;
                //mPhysics[0].velocity = {};
            }
        }   
    }

    ImGuiIO& io = ImGui::GetIO();

    if(message.button == fea::Mouse::LEFT)
        io.MouseDown[0] = true;
    else if(message.button == fea::Mouse::RIGHT)
        io.MouseDown[1] = true;
}

void Space::handleMessage(const MouseReleaseMessage& message)
{
    if(!mGuiBlocksMouse)
    {
        mInterfaceLogic.worldMouseRelease(message.position, message.position / 32, message.button);
    }

    ImGuiIO& io = ImGui::GetIO();

    if(message.button == fea::Mouse::LEFT)
        io.MouseDown[0] = false;
    else if(message.button == fea::Mouse::RIGHT)
        io.MouseDown[1] = false;
}

void Space::handleMessage(const MouseMoveMessage& message)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = (glm::vec2)message.position;

    auto position = message.position / 32;
    position.x = std::max(position.x, 0);
    position.y = std::max(position.y, 0);

    if(!mGuiBlocksMouse)
    {
        if(message.drag)
            mInterfaceLogic.worldMouseDrag(message.position, message.position / 32, fea::Mouse::LEFT);
    }
}

void Space::handleMessage(const MouseWheelMessage& message)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = static_cast<float>(message.delta);
}

void Space::startScenario()
{
	std::vector<int32_t> toRemove;
	forEach([&] (const int32_t id, const Position&)
    {
		toRemove.push_back(id);
    }, mData.tPosition);
	
	for(int32_t id : toRemove)
    {
		mActorLogic.removeActor(id);
    }
	
	for(auto position : std::vector<glm::vec2>{{240.0f, 240.0f}, {260.0f, 240.0f}, {240.0f, 260.0f}, {260.0f, 260.0f} })
    {
        auto actor = mInstantiator.instantiate("engineer", position);
        mActorLogic.addActor(std::move(actor));
    }

    mData.walls.fill(0);
    mData.atmosphere.fill(mData.defaultAtmosphere);
    glm::ivec2 offset(7, 7);
    setWall({offset + glm::ivec2(0, 0), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(1, 0), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(1, 1), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(2, 0), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(0, 2), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(1, 2), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(2, 2), Orientation::Horizontal}, 1, mData);
    setWall({offset + glm::ivec2(0, 0), Orientation::Vertical}, 1, mData);
    setWall({offset + glm::ivec2(0, 1), Orientation::Vertical}, 1, mData);
    setWall({offset + glm::ivec2(3, 0), Orientation::Vertical}, 1, mData);
    setWall({offset + glm::ivec2(3, 1), Orientation::Vertical}, 1, mData);
    setWall({offset + glm::ivec2(2, 0), Orientation::Vertical}, 1, mData);

    createDoor(Door{{offset + glm::ivec2(1, 0), Orientation::Vertical}}, mData);
    createDoor(Door{{{9, 8}, Orientation::Horizontal}}, mData);
    createDoor(Door{{{10, 7}, Orientation::Vertical}}, mData);

    setAtmosphere(offset + glm::ivec2(0, 0), cHealthyAtmosphere, mData);
    setAtmosphere(offset + glm::ivec2(1, 0), cHealthyAtmosphere, mData);
    setAtmosphere(offset + glm::ivec2(2, 0), cHealthyAtmosphere, mData);
    setAtmosphere(offset + glm::ivec2(0, 1), cHealthyAtmosphere, mData);
    setAtmosphere(offset + glm::ivec2(1, 1), cHealthyAtmosphere, mData);
    setAtmosphere(offset + glm::ivec2(2, 1), cHealthyAtmosphere, mData);

    createStructure(Structure{Structure::Airlock, offset + glm::ivec2(2, 0)}, mData);
    createStructure(Structure{Structure::CryoPods, offset + glm::ivec2(0, 0)}, mData);
    createStructure(Structure{Structure::CryoPods, offset + glm::ivec2(0, 1)}, mData);
    createStructure(Structure{Structure::Battery, offset + glm::ivec2(1, 1)}, mData);
    createStructure(Structure{Structure::Toilet, offset + glm::ivec2(1, 0)}, mData);
    int32_t crateId = createStructure(Structure{Structure::Crate, offset + glm::ivec2(2, 1)}, mData);
    int32_t containerId = get(crateId, mData.tCrate).containerId;
    createItemInContainer(Item{Item::SpaceSuit, 100}, containerId, mData);
    createItemInContainer(Item{Item::SpaceSuit, 100}, containerId, mData);
    createItemInContainer(Item{Item::SpaceSuit, 100}, containerId, mData);
    createItemInContainer(Item{Item::SpaceSuit, 100}, containerId, mData);
    //mData.atmosphere.set(offset + glm::ivec2(-1, -1), cWtfAtmosphere);


    clear(mData.tRoomTask);
    clear(mData.tWallTask);
    clear(mData.tDoorTask);
    clear(mData.unassignedTasks);
    clear(mData.tAssignedTask);

    mZoneLogic.update(mData.walls, mData.wallChanges);
    mData.atmosphereNeighbors = findAllNeighbors(mData.atmosphere, mData.walls);
    mAtmosphereLogic.scanActive();
}

void Space::loop()
{
    //grab input
    mInputHandler.process();

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    mGuiBlocksMouse = io.WantCaptureMouse;
    io.DeltaTime = 1.0f/60.0f;

    mRenderLogic.frameStart();

    for(int32_t i = 0; i < mGameSpeedMultiplier || mStepAmount > 0; ++i)
    {
        temp();
        mActorLogic.update();
        mOrganismLogic.update();
        mDecisionAILogic.update();
        mActionLogic.update();
        mStructureLogic.update();
        mTaskLogic.update();
        mZoneLogic.update(mData.walls, mData.wallChanges);
        mStructureLogic.updateAfterWall();
        updateNeighbors(mData.atmosphereNeighbors, mData.atmosphere, mData.walls, mData.wallChanges);
        invalidatePaths(mData);
        mAtmosphereLogic.update(mData.atmosphereNeighbors);

        if(mStepAmount > 0)
            --mStepAmount;

        mData.wallChanges.clear();
    }

    ImGui::ShowTestWindow();

    DebugGui::showDataTables(mClickedEntity,
            mData.tPosition, mData.tPhysics, mData.tCollisionBox, mData.tWalkTarget, mData.tMoveAbility, mData.tMoveIntention, mData.tBloodValues, mData.tChoking,
            mData.tAi, mData.uninitializedAis, mData.humanAis,
            mData.tIncentive, mData.activeIncentives, mData.tBreatheIncentive, mData.tWorkIncentive,
            mData.tAction, mData.leafActions, mData.tTaskAction, mData.tGotoAction, mData.tTotalPanicAction, mData.tFindWorkTaskAction, mData.tConstructWallAction, mData.tConstructDoorAction,
            mData.tStructureType, mData.tStructure, mData.uninitializedStructures,
            mData.tAirlock, mData.tAirlockActivity,
            mData.tDoor, mData.tStructureDoorLock, mData.tZoneLeak, mData.openDoors, mData.lockedDoors,
            mData.tTask, mData.tRoomTask, mData.tWallTask, mData.tDoorTask, mData.unassignedTasks, mData.tAssignedTask,
            mData.tItem, mData.tWearable, mData.tItemContainer, mData.tItemStoring, mData.tAirTank,
            mData.tPath,
            mData.builders, mData.freeWorkers, mData.tBusyWorker, mData.deadWorkers, mData.tActorSprite);

    DebugGui::showInspector(io.MousePos, mData.zones, mData.atmosphere);
    if(mClickedEntity)
        dbg::set<int32_t>("selected_actor", *mClickedEntity);

    mRenderLogic.update();
    mInterfaceLogic.update();

    mRenderLogic.frameEnd();

    mWindow.swapBuffers();
}

void Space::temp()
{
    forEach([&] (int32_t id, const Door& door)
    {
        if(!(rand() % 60))
        {
            closeDoor(id, mData);
        }
    }, mData.tDoor);
}
