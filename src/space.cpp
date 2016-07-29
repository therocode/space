#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "tableutil.hpp"
#include "logic/move.hpp"
#include "logic/physics.hpp"

Space::Space() :
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(1366, 768), "Space"),
    mFeaRenderer(fea::Viewport({1366, 768}, {0, 0}, fea::Camera({1366 / 2.0f, 768 / 2.0f}))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInstantiator(mResources),
    mInputHandler(mBus, mFeaInputHandler),
    //mAudioPlayer(mBus),
    mRenderer(mFeaRenderer, mResources.textures())
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);
}

void Space::handleMessage(const QuitMessage& message)
{
    (void)message;
    quit();
}

void Space::handleMessage(const MouseClickMessage& message)
{
    if(message.button == fea::Mouse::LEFT)
    {   
        auto actor = mInstantiator.instantiate("engineer", mActorIdPool.next(), message.position);
        int32_t added = addActor(std::move(actor));

        //insert(added, {{1.0f, 0.0f}, 1.0f}, mTMoveIntention);
    }   
    else if(message.button == fea::Mouse::RIGHT)
    {
        //mPositions[0].position = message.position;
        //mPhysics[0].velocity = {};
    }   
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


void Space::loop()
{
    mInputHandler.process();
    //mAudioPlayer.update();

    applyMoveIntention(mTMoveIntention, mTMoveAbility, mTPhysics);
    applyPhysics(mTPosition, mTPhysics);

    mRenderer.startFrame();
    mRenderer.renderWorld();
    renderSprites();

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
                    //sprite.rotation,
                    //sprite.animationProgress,
                    //sprite.flip,
                    }   
                    );  
            }); 

    mRenderer.render(orders);
}
