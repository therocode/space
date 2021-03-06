#pragma once
#include <fea/render2d.hpp>
#include <thero/optional.hpp>
#include "glm.hpp"
#include "pi.hpp"

enum class FillType { Solid, Hollow };

struct RenderOrder
{ 
    glm::vec2 position;
    th::Optional<int32_t> textureId;
    fea::Color color;
    th::Optional<glm::vec2> size;
    FillType fillType;
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
