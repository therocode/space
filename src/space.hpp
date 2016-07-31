#include <fea/structure.hpp>
#include <fea/render2d.hpp>
#include <fea/util.hpp>
#include <fea/userinterface.hpp>
#include "inputhandler.hpp"
#include "messages.hpp"
#include "audioplayer.hpp"
#include "renderer.hpp"
#include "resourcemanager.hpp"
#include "instantiator.hpp"
#include "data.hpp"
#include "gamecontroller.hpp"
//#include "http/httpdebugger.hpp"

class Space : public fea::Application,
    public fea::MessageReceiver<QuitMessage,
    MouseClickMessage,
    MouseReleaseMessage,
    MouseMoveMessage,
    MouseWheelMessage>
{
    public:
        Space();
        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const MouseClickMessage& message) override;
        void handleMessage(const MouseReleaseMessage& message) override;
        void handleMessage(const MouseMoveMessage& message) override;
        void handleMessage(const MouseWheelMessage& message) override;
        int32_t addActor(Actor actor);
    protected:
        void loop() override;
    private:
        void renderSprites();

        //fea
        fea::MessageBus mBus;
        fea::Window mWindow;
        fea::Renderer2D mFeaRenderer;
        fea::InputHandler mFeaInputHandler;

        //resources
        ResourceManager mResources;
        Instantiator mInstantiator;

        //input
        InputHandler mInputHandler;
        
        //rendering
        Renderer mRenderer;

        //game data
        ent::TPosition mTPosition;
        ent::TPhysics mTPhysics;
        ent::TWalkTarget mTWalkTarget;
        ent::TMoveIntention mTMoveIntention;
        ent::TMoveAbility mTMoveAbility;
        ////worker stuff
        IdSet mBuilders;
        IdSet mFreeWorkers;

        ////tasks
        tsk::TRoomTask mTRoomTask;
        tsk::TWallTask mTWallTask;
        tsk::TDoorTask mTDoorTask;

        gfx::TActorSprite mTActorSprite;

        NumberPool<int32_t> mActorIdPool;
        NumberPool<int32_t> mActorSpriteIdPool;
        //AudioPlayer mAudioPlayer;
        
        //HttpDebugger mHttpDebugger;
        fea::Texture mImguiFontTexture;

        GameController mController;
        bool mGuiBlocksMouse;
};
