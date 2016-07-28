#include <algorithm>
#include <thero/assert.hpp>
#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "debugrenderer.hpp"

const fea::Color cGroundColor = {83, 75, 57};
Renderer::Renderer(fea::Renderer2D& renderer, const std::vector<Texture>& textures):
    mRenderer(renderer),
    mTextures(textures)
{
    DRen::initialize(mRenderer, mRenderer.getViewport().getCamera());
}

void Renderer::startFrame()
{
    mRenderer.clear();
}

void Renderer::renderWorld()
{
    mRenderer.clear(cGroundColor);
}

void Renderer::render(const std::vector<RenderOrder>& orders) const
{
    for(const auto& renderOrder : orders)
    {
        const auto textureIter = std::find_if(mTextures.begin(), mTextures.end(), [&renderOrder] (const Texture& disp) { return disp.textureId == renderOrder.textureId; });
        TH_ASSERT(textureIter != mTextures.end(), "render order uses invalid texture " << renderOrder.textureId);
        const Texture& texture = *textureIter;

        fea::AnimatedQuad quad(texture.size);

        if(texture.texture)
            quad.setTexture(*texture.texture);
        //if(texture.animation)
        //{
        //    quad.setAnimation(*texture.animation);
        //    quad.setAnimationFrame(static_cast<uint32_t>(renderOrder.animationProgress) / texture.animation->getDelay() % texture.animation->getFrameAmount());
        //}


        //auto offset = texture.offset;
        //if(renderOrder.flip)
        //{
        //    offset.x *= -1.0f;
        //    offset.x += -texture.size.x;
        //}

        quad.setPosition(renderOrder.position);// + offset);
        //quad.setRotation(renderOrder.rotation - pi / 2.0f); //take away pi/2.0f which is a quarter of a full turn because featherkit expects 0 degrees to be --> but in the game, it is downwards
        //quad.setHFlip(renderOrder.flip);
        quad.setColor(renderOrder.color);

        mRenderer.render(quad);
    }

    DRen::flush();
}
