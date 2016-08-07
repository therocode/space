#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "tableutil.hpp"
#include "logic/move.hpp"
#include "logic/physics.hpp"
#include "debugguidata.hpp"
#include "debuggui.hpp"
#include <imgui.h>
#include "drawables/linerect.hpp"

Space::Space() :
    mWindowSize(1366, 768),
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(static_cast<uint32_t>(mWindowSize.x), static_cast<uint32_t>(mWindowSize.y)), "Space"),
    mFeaRenderer(fea::Viewport(mWindowSize, {0, 0}, fea::Camera(static_cast<glm::vec2>(mWindowSize / 2)))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInstantiator(mResources),
    mInputHandler(mBus, mFeaInputHandler),
    //mAudioPlayer(mBus),
    mRenderer(mFeaRenderer, mResources.textures()),
    mGuiBlocksMouse(false)
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

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
        mController.worldMouseClick(message.position, message.position / 32, message.button);

        if(message.button == fea::Mouse::LEFT)
        {   
            auto actor = mInstantiator.instantiate("engineer", mActorIdPool.next(), message.position);
            int32_t added = addActor(std::move(actor));

            insert(added, {50.0f, 50.0f}, mTWalkTarget);
            //insert(added, {{1.0f, 0.0f}, 1.0f}, mTMoveIntention);
        }   
        else if(message.button == fea::Mouse::RIGHT)
        {
            int32_t toDelete = rand() % count(mTPosition);
            if(has(toDelete, mTPosition))
            {
                removeActor(toDelete);
                mActorIdPool.release(toDelete);
            }
            //mPositions[0].position = message.position;
            //mPhysics[0].velocity = {};
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
        mController.worldMouseRelease(message.position, message.position / 32, message.button);
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
        mController.worldMouseDrag(message.position, message.position / 32, fea::Mouse::LEFT);
}

void Space::handleMessage(const MouseWheelMessage& message)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel = static_cast<float>(message.delta);
}

int32_t Space::addActor(Actor actor)
{
    int32_t id = actor.id;
    tableEmplaceOptional(id, std::move(actor.position), mTPosition);
    tableEmplaceOptional(id, std::move(actor.physics), mTPhysics);
    tableEmplaceOptional(id, std::move(actor.moveAbility), mTMoveAbility);

    for(auto& sprite : actor.actorSprites)
    {   
        tableEmplaceOptional(mActorSpriteIdPool.next(), std::move(sprite), mTActorSprite);
    }   

    if(actor.worker)
    {
        insert(id, mBuilders);
        insert(id, mFreeWorkers);
    }

    return id;
}

void Space::removeActor(int32_t id)
{
    erase(id, mTPosition);
    erase(id, mTPhysics);
    erase(id, mTMoveAbility);
    erase(id, mTMoveIntention);
    erase(id, mTWalkTarget);
    eraseIf([&] (int32_t actorSpriteId, const ActorSprite& actorSprite)
    {
        if(actorSprite.actorId == id)
        {
            mActorSpriteIdPool.release(actorSpriteId);
            return true;
        }
        return false;
    }, mTActorSprite);
    erase(id, mBuilders);
    erase(id, mFreeWorkers);
}

void Space::loop()
{
    mInputHandler.process();

    //imgui
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = 1.0f/60.0f;
    //io.MousePos = mouse_pos;
    //io.MouseDown[0] = mouse_button_0;
    //io.MouseDown[1] = mouse_button_1;
    //io.KeysDown[i] = ...
    ImGui::NewFrame();

    mGuiBlocksMouse = io.WantCaptureMouse;
    //mAudioPlayer.update();

    handleControllerOutput(mController.fetchOutput());

    calculateMoveIntention(mTWalkTarget, mTMoveIntention, mTPosition);
    applyMoveIntention(mTMoveIntention, mTMoveAbility, mTPhysics);
    applyPhysics(mTPosition, mTPhysics);

    mRenderer.startFrame();
    mRenderer.renderWorld();
    renderTasks();
    renderSprites();

    mController.updateAndRender(mFeaRenderer);

    ImGui::ShowTestWindow();
    auto clickedId = DebugGui::showDataTables(mTPosition, mTPhysics, mTWalkTarget, mTMoveAbility, mTMoveIntention, mTRoomTask, mTDoorTask, mTActorSprite);
    if(clickedId) mSelectedDebugEntity = clickedId;
    ImGui::Render();
    mRenderer.renderImGui(*ImGui::GetDrawData());

    mWindow.swapBuffers();
}

void Space::renderSprites()
{
    std::vector<RenderOrder> orders;

    forEach(mTActorSprite, [&] (int32_t actorId, const ActorSprite& sprite)
    {   
        const glm::vec2& position = get(sprite.actorId, mTPosition).data;

        orders.emplace_back(
                RenderOrder{
                position,
                sprite.textureId,
                sprite.color,
                {}
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  

        if(mSelectedDebugEntity && *mSelectedDebugEntity == actorId)
        {
            const auto textureIter = std::find_if(mResources.textures().begin(), mResources.textures().end(), [&] (const Texture& disp) { return disp.textureId == sprite.textureId; });

            LineRect rect(textureIter->size);
            rect.setPosition(position);
            rect.setColor(fea::Color::Yellow);
            mFeaRenderer.render(rect);
        }
    }); 

    mRenderer.render(orders);
}

void Space::renderTasks()
{
    std::vector<RenderOrder> orders;

    forEach(mTRoomTask, [&] (int32_t taskId, const RoomTask& roomTask)
    {   
        const glm::vec2& position = roomTask.position * 32;
        const glm::vec2& size = roomTask.size * 32;

        orders.emplace_back(
                RenderOrder{
                position,
                {},
                fea::Color(50, 150, 0, 200),
                size,
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  
    }); 

    mRenderer.render(orders);
}

void Space::handleControllerOutput(GameController::Output output)
{
    for(auto& roomTask : output.roomTasks)
    {
        insert(mTaskIdPool.next(), std::move(roomTask), mTRoomTask);
    }
}
