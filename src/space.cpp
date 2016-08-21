#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "debugguidata.hpp"
#include "debuggui.hpp"
#include "debug.hpp"
#include "roomutil.hpp"
#include "wallutil.hpp"
#include "doorutil.hpp"
#include <imgui.h>

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
    mGameSpeedMultiplier(4),
    mShowZones(false),
    mShowAtmosphere(false),
    mWalls(cMapSize),
	mOldWalls(cMapSize),
    mAtmosphere(cMapSize, cDefaultAtmosphere),
    mGuiBlocksMouse(false),
    mActorLogic(mEnt, mGfx, mTsk, mWld, mWalls),
    mOrganismLogic(mEnt, mAtmosphere),
    mTaskLogic(mTsk, mEnt, mWld, mWalls),
    mZoneLogic(mZones),
    mAtmosphereLogic(mZones, mWalls, mAtmosphere),
    mRenderLogic(mResources, mFeaRenderer, mWalls, mZones, mAtmosphere, mGfx, mEnt, mTsk, mWld, mShowZones, mShowAtmosphere),
    mInterfaceLogic(*this, mFeaRenderer, mGameSpeedMultiplier, mShowZones, mShowAtmosphere, mTaskIdPool, mWalls, mTsk, mEnt)
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

    init(cMapSize, 0, mZones);

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = mWindowSize.x;
    io.DisplaySize.y = mWindowSize.y;
    io.IniFilename = "data/imgui.ini";
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    mImguiFontTexture.create({width, height}, pixels);
    io.Fonts->TexID = reinterpret_cast<void*>(mImguiFontTexture.getId());
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
            if(count(mEnt.tPosition))
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
    }, mEnt.tPosition);
	
	for(int32_t id : toRemove)
    {
		mActorLogic.removeActor(id);
    }
	
	for(auto position : std::vector<glm::vec2>{{240.0f, 240.0f}, {260.0f, 240.0f}, {240.0f, 260.0f}, {260.0f, 260.0f} })
    {
        auto actor = mInstantiator.instantiate("engineer", position);
        mActorLogic.addActor(std::move(actor));
    }

    mWalls.fill(0);
    mAtmosphere.fill(cDefaultAtmosphere);
    glm::ivec2 offset(7, 7);
    mWalls.set(offset + glm::ivec2(0, 0), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(1, 0), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(2, 0), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(0, 2), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(1, 2), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(2, 2), Orientation::Horizontal, 1);
    mWalls.set(offset + glm::ivec2(0, 0), Orientation::Vertical, 1);
    mWalls.set(offset + glm::ivec2(0, 1), Orientation::Vertical, 1);
    mWalls.set(offset + glm::ivec2(3, 0), Orientation::Vertical, 1);
    mWalls.set(offset + glm::ivec2(3, 1), Orientation::Vertical, 1);
    mWalls.set(offset + glm::ivec2(2, 0), Orientation::Vertical, 1);

    createDoor(Door{{9, 8}, Orientation::Horizontal}, mWld.tDoor, mWld.openDoors, mWalls);
    createDoor(Door{{10, 7}, Orientation::Vertical}, mWld.tDoor, mWld.openDoors, mWalls);
    createDoor(Door{{11, 7}, Orientation::Vertical}, mWld.tDoor, mWld.openDoors, mWalls);
    createDoor(Door{{12, 7}, Orientation::Vertical}, mWld.tDoor, mWld.openDoors, mWalls);
    createDoor(Door{{13, 7}, Orientation::Vertical}, mWld.tDoor, mWld.openDoors, mWalls);
    createDoor(Door{{14, 7}, Orientation::Vertical}, mWld.tDoor, mWld.openDoors, mWalls);

    //temp
    mWalls.set(glm::ivec2(10, 7), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(10, 8), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(11, 7), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(11, 8), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(12, 7), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(12, 8), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(13, 7), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(13, 8), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(14, 7), Orientation::Horizontal, 1);
    mWalls.set(glm::ivec2(14, 8), Orientation::Horizontal, 1);
    mAtmosphere.set(glm::ivec2(10, 7), cHealthyAtmosphere);
    mAtmosphere.set(glm::ivec2(11, 7), cHealthyAtmosphere);
    mAtmosphere.set(glm::ivec2(12, 7), cHealthyAtmosphere);
    mAtmosphere.set(glm::ivec2(13, 7), cHealthyAtmosphere);
    mAtmosphere.set(glm::ivec2(14, 7), cHealthyAtmosphere);
    //endtemp

    mAtmosphere.set(offset + glm::ivec2(0, 0), cHealthyAtmosphere);
    mAtmosphere.set(offset + glm::ivec2(1, 0), cHealthyAtmosphere);
    mAtmosphere.set(offset + glm::ivec2(2, 0), cHealthyAtmosphere);
    mAtmosphere.set(offset + glm::ivec2(0, 1), cHealthyAtmosphere);
    mAtmosphere.set(offset + glm::ivec2(1, 1), cHealthyAtmosphere);
    mAtmosphere.set(offset + glm::ivec2(2, 1), cHealthyAtmosphere);


    //mAtmosphere.set(offset + glm::ivec2(-1, -1), cWtfAtmosphere);


    clear(mTsk.tRoomTask);
    clear(mTsk.tWallTask);
    clear(mTsk.tDoorTask);
    clear(mTsk.unassignedTasks);
    clear(mTsk.tAssignedTask);
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
        mTaskLogic.update();
        auto wallChanges = wallDiff(mOldWalls, mWalls);
        mZoneLogic.update(mOldWalls, wallChanges);
        mAtmosphereLogic.update();
        mOldWalls = mWalls;
    }
	mOldWalls = mWalls;

    ImGui::ShowTestWindow();
    DebugGui::showDataTables(mClickedEntity, mEnt.tPosition, mEnt.tPhysics, mEnt.tWalkTarget, mEnt.tMoveAbility, mEnt.tMoveIntention, mEnt.tBloodValues, mEnt.tChoking, mTsk.tRoomTask, mTsk.tWallTask, mTsk.tDoorTask, mTsk.unassignedTasks, mTsk.tAssignedTask, mEnt.builders, mEnt.freeWorkers, mEnt.tBusyWorker, mEnt.deadWorkers, mGfx.tActorSprite);
    DebugGui::showInspector(io.MousePos, mZones, mAtmosphere);
    if(mClickedEntity)
        dbg::set<int32_t>("selected_actor", *mClickedEntity);

    mRenderLogic.update();
    mInterfaceLogic.update();

    mRenderLogic.frameEnd();

    mWindow.swapBuffers();
}

//TODO:
//wall collision
//pathfinding
//door opening
//task dependencies

//refact:
//get rid of double rendering layer
//group up system variables

void Space::temp()
{
    forEach([&] (int32_t id, const Door& door)
    {
        if(!(rand() % 60))
        {
            toggleDoor(id, mWld.tDoor, mWld.openDoors, mWalls);
        }
    }, mWld.tDoor);
}
