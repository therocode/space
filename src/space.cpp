#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "debugguidata.hpp"
#include "debuggui.hpp"
#include "debug.hpp"
#include "roomutil.hpp"
#include "wallutil.hpp"
#include <imgui.h>

const glm::ivec2 cMapSize(256, 256);

Space::Space() :
    mWindowSize(1366, 768),
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(static_cast<uint32_t>(mWindowSize.x), static_cast<uint32_t>(mWindowSize.y)), "Space"),
    mFeaRenderer(fea::Viewport(mWindowSize, {0, 0}, fea::Camera(static_cast<glm::vec2>(mWindowSize / 2)))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInstantiator(mResources),
    mInputHandler(mBus, mFeaInputHandler),
    mGameSpeedMultiplier(4),
    //mAudioPlayer(mBus),
    mWalls(cMapSize),
    mGuiBlocksMouse(false),
    mActorLogic(mTPosition, mTPhysics, mTMoveAbility, mTMoveIntention, mTWalkTarget, mTActorSprite, mBuilders, mFreeWorkers, mTBusyWorker, mTAssignedTask, mUnassignedTasks),
    mTaskLogic(mWalls, mTRoomTask, mTWallTask, mTDoorTask, mUnassignedTasks, mTAssignedTask),
    mZoneLogic(mWalls, mZones),
    mRenderLogic(mResources, mFeaRenderer, mWalls, mZones, mTActorSprite, mTPosition, mTRoomTask, mTWallTask),
    mInterfaceLogic(mFeaRenderer, mGameSpeedMultiplier, mTaskIdPool, mWalls, mTRoomTask, mTWallTask, mUnassignedTasks)
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

    init(cMapSize, 0, mZones);

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = mWindowSize.x;
    io.DisplaySize.y = mWindowSize.y;
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
            auto actor = mInstantiator.instantiate("engineer", mActorIdPool.next(), message.position);
            int32_t added = mActorLogic.addActor(std::move(actor));
        }   
        else if(message.button == fea::Mouse::RIGHT)
        {
            if(count(mTPosition))
            {
                int32_t toDelete = rand() % count(mTPosition);
                if(has(toDelete, mTPosition))
                {
                    mActorLogic.removeActor(toDelete);
                    mActorIdPool.release(toDelete);
                }
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

    if(message.drag)
        mInterfaceLogic.worldMouseDrag(message.position, message.position / 32, fea::Mouse::LEFT);
}

void Space::handleMessage(const MouseWheelMessage& message)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = static_cast<float>(message.delta);
}

void Space::loop()
{
    WallMap oldWalls = mWalls;
    //grab input
    mInputHandler.process();

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    mGuiBlocksMouse = io.WantCaptureMouse;
    io.DeltaTime = 1.0f/60.0f;

    mRenderLogic.frameStart();

    for(int32_t i = 0; i < mGameSpeedMultiplier; ++i)
    {
        mActorLogic.update();
        mTaskLogic.update();
        auto wallChanges = wallDiff(oldWalls, mWalls);
        mZoneLogic.update(wallChanges);
        oldWalls = mWalls;
    }

    ImGui::ShowTestWindow();
    auto clickedId = DebugGui::showDataTables(mTPosition, mTPhysics, mTWalkTarget, mTMoveAbility, mTMoveIntention, mTRoomTask, mTWallTask, mTDoorTask, mUnassignedTasks, mTAssignedTask, mBuilders, mFreeWorkers, mTBusyWorker, mTActorSprite);
    if(clickedId)
        dbg::set<int32_t>("selected_actor", *clickedId);

    mRenderLogic.update();
    mInterfaceLogic.update();

    mRenderLogic.frameEnd();

    //temp();

    mWindow.swapBuffers();
}

//TODO:
//dudes should not be dangling busy workers
//dudes working on wall building tasks should build them
//option for internal/external book keeping in tables
//task dependencies

void Space::temp()
{
    forEach([this] (int32_t id, const RoomTask& roomTask)
    {
        const auto& position = roomTask.position;
        const auto& size = roomTask.size;

        forEachWall(position, size, [this] (const glm::ivec2& coordinate, Orientation orientation)
        {
            mWalls.set(coordinate, orientation, 1);
        });

        forEachFloor(position, size, [&] (const glm::ivec2& coordinate)
        {
            set(coordinate, id+1, mZones); //+1 to make it not zero
        });

    }, mTRoomTask);

    clear(mTRoomTask);
}
