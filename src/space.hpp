#include <fea/structure.hpp>
#include <fea/util.hpp>
#include <fea/userinterface.hpp>
#include <fea/render2d.hpp>
#include "inputhandler.hpp"
#include "messages.hpp"
#include "audioplayer.hpp"
#include "resourcemanager.hpp"
#include "instantiator.hpp"
#include "data.hpp"
#include "wallmap.hpp"
#include "zones.hpp"
#include "logic/interfacelogic.hpp"
#include "logic/actorlogic.hpp"
#include "logic/tasklogic.hpp"
#include "logic/zonelogic.hpp"
#include "logic/renderlogic.hpp"

class Space : public fea::Application,
    public fea::MessageReceiver<QuitMessage,
    ResizeMessage,
    KeyPressedMessage,
    MouseClickMessage,
    MouseReleaseMessage,
    MouseMoveMessage,
    MouseWheelMessage>
{
    public:
        Space();
        void handleMessage(const QuitMessage& message) override;
        void handleMessage(const ResizeMessage& message) override;
        void handleMessage(const KeyPressedMessage& message) override;
        void handleMessage(const MouseClickMessage& message) override;
        void handleMessage(const MouseReleaseMessage& message) override;
        void handleMessage(const MouseMoveMessage& message) override;
        void handleMessage(const MouseWheelMessage& message) override;
    protected:
        void loop() override;
    private:

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

        //system
        int32_t mGameSpeedMultiplier;
        bool mShowZones;

        //game data
        ent::TPosition mTPosition = {"Position", "The positions of game entities"};
        ent::TPhysics mTPhysics = {"Physics", "The acceleration and velocities of game entities"};
        ent::TWalkTarget mTWalkTarget = {"Walk Target", "The positions that game entities seek to move to"};
        ent::TMoveIntention mTMoveIntention = {"Move Intention", "The directions and speeds that game entities which to attain"};
        ent::TMoveAbility mTMoveAbility = {"Move Ability", "How well game entities are capable of moving"};

        ////worker stuff
        IdSet mBuilders = {{}, {"Builders", "All workers who are builders"}};
        IdSet mFreeWorkers = {{}, {"Free workers", "All workers who are currently not working on any task"}};
        ent::TBusyWorker mTBusyWorker = {"Busy Worker", "The workers who are working on a task"};

        ////tasks
        tsk::TRoomTask mTRoomTask = {"Room Task", "Represent all rooms that need to be built"};
        tsk::TWallTask mTWallTask = {"Wall Task", "Represent all walls that need to be built"};
        tsk::TDoorTask mTDoorTask = {"Door Task", "Represent all doors that need to be built"};
        IdSet mUnassignedTasks = {{}, {"Unassigned tasks", "All tasks which no worker is currently on"}};
        tsk::TAssignedTask mTAssignedTask = {"Assigned tasks", "All tasks which are assigned to a worker"};

        //world
        WallMap mWalls;
        Zones mZones;

        //gfx
        gfx::TActorSprite mTActorSprite = {"Actor Sprite", "Many-to-many relationship between game objects and sprites. Represents the visibility on screen of game entities"};

        NumberPool<int32_t> mActorIdPool;
        NumberPool<int32_t> mTaskIdPool;
        //AudioPlayer mAudioPlayer;
        
        //HttpDebugger mHttpDebugger;
        fea::Texture mImguiFontTexture;

        bool mGuiBlocksMouse;

        //logic
        ActorLogic mActorLogic;
        TaskLogic mTaskLogic;
        ZoneLogic mZoneLogic;
        RenderLogic mRenderLogic;
        InterfaceLogic mInterfaceLogic;

        void temp();
};
