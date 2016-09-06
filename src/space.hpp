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
#include "gases.hpp"
#include "logic/interfacelogic.hpp"
#include "logic/actorlogic.hpp"
#include "logic/organismlogic.hpp"
#include "logic/decisionailogic.hpp"
#include "logic/structurelogic.hpp"
#include "logic/tasklogic.hpp"
#include "logic/zonelogic.hpp"
#include "logic/atmospherelogic.hpp"
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
		void startScenario();
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
        bool mShowAtmosphere;
        th::Optional<int32_t> mClickedEntity;

        GameData mData;

        NumberPool<int32_t> mTaskIdPool;
        //AudioPlayer mAudioPlayer;
        
        //HttpDebugger mHttpDebugger;
        fea::Texture mImguiFontTexture;

        bool mGuiBlocksMouse;

        //logic
        ActorLogic mActorLogic;
        OrganismLogic mOrganismLogic;
        DecisionAILogic mDecisionAILogic;
        StructureLogic mStructureLogic;
        TaskLogic mTaskLogic;
        ZoneLogic mZoneLogic;
        AtmosphereLogic mAtmosphereLogic;
        RenderLogic mRenderLogic;
        InterfaceLogic mInterfaceLogic;

        void temp();
};
