#include <algorithm>
#include <thero/assert.hpp>
#include <fea/rendering/opengl.hpp>
#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "debugrenderer.hpp"
#include "drawables/imguidrawable.hpp"

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

void Renderer::renderImGui(ImDrawData& drawData)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    drawData.ScaleClipRects(io.DisplayFramebufferScale);

    glEnable(GL_SCISSOR_TEST);
   
    //// Render command lists
    for (int n = 0; n < drawData.CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = drawData.CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                ImGuiDrawable drawable(vtx_buffer, idx_buffer, static_cast<int32_t>(pcmd->ElemCount), pcmd->TextureId);
                mRenderer.render(drawable);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }

    glDisable(GL_SCISSOR_TEST);
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
