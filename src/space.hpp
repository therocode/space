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
    ResizeMessage,
    MouseClickMessage,
    MouseReleaseMessage,
    MouseMoveMessage,
    MouseWheelMessage>
{
    public:
        Space();
        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;
        void handleMessage(const MouseClickMessage& message) override;
        void handleMessage(const MouseReleaseMessage& message) override;
        void handleMessage(const MouseMoveMessage& message) override;
        void handleMessage(const MouseWheelMessage& message) override;
        int32_t addActor(Actor actor);
        void removeActor(int32_t id);
    protected:
        void loop() override;
    private:
        void renderSprites();
        void renderTasks();
        void handleControllerOutput(GameController::Output output);

        glm::ivec2 mWindowSize;
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
        ent::TPosition mTPosition = {"Position", "The positions of game entities"};
        ent::TPhysics mTPhysics = {"Physics", "The acceleration and velocities of game entities"};
        ent::TWalkTarget mTWalkTarget = {"Walk Target", "The positions that game entities seek to move to"};
        ent::TMoveIntention mTMoveIntention = {"Move Intention", "The directions and speeds that game entities which to attain"};
        ent::TMoveAbility mTMoveAbility = {"Move Ability", "How well game entities are capable of moving"};
        ////worker stuff
        IdSet mBuilders;
        IdSet mFreeWorkers;

        ////tasks
        tsk::TRoomTask mTRoomTask = {"Room Task", "Represent all rooms that need to be built"};
        tsk::TWallTask mTWallTask = {"Wall Task", "Represent all walls that need to be built"};
        tsk::TDoorTask mTDoorTask = {"Door Task", "Represent all doors that need to be built"};

        gfx::TActorSprite mTActorSprite = {"Actor Sprite", "Many-to-many relationship between game objects and sprites. Represents the visibility on screen of game entities"};

        NumberPool<int32_t> mActorIdPool;
        NumberPool<int32_t> mActorSpriteIdPool;
        NumberPool<int32_t> mTaskIdPool;
        //AudioPlayer mAudioPlayer;
        
        //HttpDebugger mHttpDebugger;
        fea::Texture mImguiFontTexture;

        GameController mController;
        bool mGuiBlocksMouse;
};
