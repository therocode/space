#include "space.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>

Space::Space() :
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(1366, 768), "Space"),
    mFeaRenderer(fea::Viewport({1366, 768}, {0, 0}, fea::Camera({1366 / 2.0f, 768 / 2.0f}))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInputHandler(mBus, mFeaInputHandler),
    mAudioPlayer(mBus),
    mRenderer(mFeaRenderer)
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

void Space::loop()
{
    mInputHandler.process();
    mAudioPlayer.update();
    mRenderer.render();

    mWindow.swapBuffers();
}
