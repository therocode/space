#pragma once
#include "glm.hpp"
#include "pi.hpp"

struct RenderOrder
{ 
    glm::vec2 position;
    int32_t textureId;
    fea::Color color;
    //float rotation = pi / 2.0f;
    //int32_t animationProgress = 0;
    //bool flip;
};
  
namespace fea
{ 
    class Texture;
    class Animation;
} 

struct Texture
{
    int32_t textureId;
    glm::vec2 size;
    glm::vec2 offset;
    const fea::Texture* texture;
    //const fea::Animation* animation;
};
