#pragma once
#include "glm.hpp"
#include <string>
#include <fea/userinterface.hpp>

struct QuitMessage
{
};

struct ResizeMessage
{
    glm::uvec2 size;
};

struct PlayMusicMessage
{
    std::string name;
    bool loop;
};

struct PlaySoundMessage
{
    std::string name;
    bool loop;
};

struct StopSoundMessage
{
};

struct MouseClickMessage
{
    fea::Mouse::Button button;
    glm::ivec2 position;
};

struct MouseReleaseMessage
{
    fea::Mouse::Button button;
    glm::ivec2 position;
};

struct MouseMoveMessage
{
    glm::ivec2 position;
    bool drag;
};

struct MouseWheelMessage
{
    int32_t delta;
};

struct KeyPressedMessage
{
    int32_t key;
};
