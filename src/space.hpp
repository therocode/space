#include <fea/structure.hpp>
#include <fea/render2d.hpp>
#include <fea/util.hpp>
#include <fea/userinterface.hpp>
#include "inputhandler.hpp"
#include "messages.hpp"
#include "audioplayer.hpp"
#include "renderer.hpp"
#include "resourcemanager.hpp"
#include "data.hpp"

class Space : public fea::Application,
    public fea::MessageReceiver<QuitMessage>
{
    public:
        Space();
        void handleMessage(const QuitMessage& message) override;
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

        //input
        InputHandler mInputHandler;
        
        //rendering
        Renderer mRenderer;

        //data
        ent::TPosition mTPosition;
        gfx::TActorSprite mTActorSprite;

        //AudioPlayer mAudioPlayer;
};
