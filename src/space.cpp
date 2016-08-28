#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "debugguidata.hpp"
#include "debuggui.hpp"
#include "debug.hpp"
#include "roomutil.hpp"
#include "doorutil.hpp"
#include "gridneighbors.hpp"
#include <imgui.h>
#include "structuretypes.hpp"

const glm::ivec2 cMapSize(256, 256);
const Gases cDefaultAtmosphere
{{
    static_cast<int32_t>(50000 * 0.0f), //oxygene
    static_cast<int32_t>(50000 * 0.0f), //nitrogen
    static_cast<int32_t>(50000 * 0.0f), //carbondioxide
}};
const Gases cHealthyAtmosphere
{{
    static_cast<int32_t>(50000 * 0.209f), //oxygene
    static_cast<int32_t>(50000 * 0.78f), //nitrogen
    static_cast<int32_t>(50000 * 0.004f), //carbondioxide
}};
const Gases cWtfAtmosphere
{{
    static_cast<int32_t>(200000000 * 0.00f), //oxygene
    static_cast<int32_t>(200000000 * 0.4f), //nitrogen
    static_cast<int32_t>(200000000 * 0.9f), //carbondioxide
}};

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
    mShowZones(false),
    mShowAtmosphere(false),
    mGuiBlocksMouse(false),
    mActorLogic(mData),
    mOrganismLogic(mData),
    mStructureLogic(mData, mResources),
    mTaskLogic(mData),
    mZoneLogic(mData),
    mAtmosphereLogic(mData),
    mRenderLogic(mResources, mFeaRenderer, mData, mShowZones, mShowAtmosphere),
    mInterfaceLogic(*this, mFeaRenderer, mGameSpeedMultiplier, mShowZones, mShowAtmosphere, mTaskIdPool, mData)
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

    mData.walls = {cMapSize};
    mData.atmosphere = {cMapSize, cDefaultAtmosphere};
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

    if(message.drag)
        mInterfaceLogic.worldMouseDrag(message.position, message.position / 32, fea::Mouse::LEFT);
}

void Space::handleMessage(const MouseWheelMessage& message)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = static_cast<float>(message.delta);
}

void Space::startScenario()
{
	std::vector<int32_t> toRemove;
	forEach([&] (const int32_t id, const glm::vec2&)
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
    mData.atmosphere.fill(cDefaultAtmosphere);
    glm::ivec2 offset(7, 7);
    set({offset + glm::ivec2(0, 0), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(1, 0), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(1, 1), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(2, 0), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(0, 2), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(1, 2), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(2, 2), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(0, 0), Orientation::Vertical}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(0, 1), Orientation::Vertical}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(3, 0), Orientation::Vertical}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(3, 1), Orientation::Vertical}, 1, mData.walls, mData.wallChanges);
    set({offset + glm::ivec2(2, 0), Orientation::Vertical}, 1, mData.walls, mData.wallChanges);

    createDoor(Door{offset + glm::ivec2(1, 0), Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{9, 8}, Orientation::Horizontal}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{10, 7}, Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{11, 7}, Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{12, 7}, Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{13, 7}, Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
    createDoor(Door{{14, 7}, Orientation::Vertical}, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);

    //temp
    set({glm::ivec2(10, 7), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(10, 8), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(11, 7), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(11, 8), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(12, 7), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(12, 8), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(13, 7), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(13, 8), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(14, 7), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    set({glm::ivec2(14, 8), Orientation::Horizontal}, 1, mData.walls, mData.wallChanges);
    mData.atmosphere.set(glm::ivec2(10, 7), cHealthyAtmosphere);
    mData.atmosphere.set(glm::ivec2(11, 7), cHealthyAtmosphere);
    mData.atmosphere.set(glm::ivec2(12, 7), cHealthyAtmosphere);
    mData.atmosphere.set(glm::ivec2(13, 7), cHealthyAtmosphere);
    mData.atmosphere.set(glm::ivec2(14, 7), cHealthyAtmosphere);
    //endtemp

    mData.atmosphere.set(offset + glm::ivec2(0, 0), cHealthyAtmosphere);
    mData.atmosphere.set(offset + glm::ivec2(1, 0), cHealthyAtmosphere);
    mData.atmosphere.set(offset + glm::ivec2(2, 0), cHealthyAtmosphere);
    mData.atmosphere.set(offset + glm::ivec2(0, 1), cHealthyAtmosphere);
    mData.atmosphere.set(offset + glm::ivec2(1, 1), cHealthyAtmosphere);
    mData.atmosphere.set(offset + glm::ivec2(2, 1), cHealthyAtmosphere);

    createStructure(Structure{offset + glm::ivec2(2, 0), Structures::Airlock}, mData);
    createStructure(Structure{offset + glm::ivec2(0, 0), Structures::CryoPods}, mData);
    createStructure(Structure{offset + glm::ivec2(0, 1), Structures::CryoPods}, mData);
    createStructure(Structure{offset + glm::ivec2(1, 1), Structures::Battery}, mData);
    createStructure(Structure{offset + glm::ivec2(1, 0), Structures::Toilet}, mData);
    createStructure(Structure{offset + glm::ivec2(2, 1), Structures::Crate}, mData);
    //mData.atmosphere.set(offset + glm::ivec2(-1, -1), cWtfAtmosphere);


    clear(mData.tRoomTask);
    clear(mData.tWallTask);
    clear(mData.tDoorTask);
    clear(mData.unassignedTasks);
    clear(mData.tAssignedTask);
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

    for(int32_t i = 0; i < mGameSpeedMultiplier; ++i)
    {
        temp();
        mActorLogic.update();
        mOrganismLogic.update();
        mStructureLogic.update();
        mTaskLogic.update();
        mZoneLogic.update(mData.walls, mData.wallChanges);
        mStructureLogic.updateAfterWall();
        updateNeighbors(mData.atmosphereNeighbors, mData.atmosphere, mData.walls, mData.wallChanges);
        mAtmosphereLogic.update(mData.atmosphereNeighbors);
    }

    ImGui::ShowTestWindow();
    DebugGui::showDataTables(mClickedEntity, mData.tPosition, mData.tPhysics, mData.tCollisionBox, mData.tWalkTarget, mData.tMoveAbility, mData.tMoveIntention, mData.tBloodValues, mData.tChoking, mData.tStructureType, mData.tStructure, mData.uninitializedStructures, mData.tAirlock, mData.tRoomTask, mData.tWallTask, mData.tDoorTask, mData.unassignedTasks, mData.tAssignedTask, mData.builders, mData.freeWorkers, mData.tBusyWorker, mData.deadWorkers, mData.tActorSprite);
    DebugGui::showInspector(io.MousePos, mData.zones, mData.atmosphere);
    if(mClickedEntity)
        dbg::set<int32_t>("selected_actor", *mClickedEntity);

    mRenderLogic.update();
    mInterfaceLogic.update();

    mRenderLogic.frameEnd();

    mWindow.swapBuffers();
}

//TODO:
//airlocks
//pathfinding
//task dependencies
//ambient gas transfer

//refact:
//get rid of double rendering layer
//group up system variables

void Space::temp()
{
    forEach([&] (int32_t id, const Door& door)
    {
        if(!(rand() % 60))
        {
            closeDoor(id, mData.tDoor, mData.openDoors, mData.walls, mData.wallChanges);
        }
    }, mData.tDoor);
}
