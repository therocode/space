#include <algorithm>
#include <thero/assert.hpp>
#include <fea/rendering/opengl.hpp>
#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "debugrenderer.hpp"
#include "drawables/imguidrawable.hpp"
#include "drawables/linerect.hpp"

const float cTileWidth = 32.0f;
const float cWallThickness = 8.0f;

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

void Renderer::renderWorld(const WallMap& walls, const Grid<int32_t>& zones, bool showZones)
{
    mRenderer.clear(cGroundColor);

    fea::Quad quad({cTileWidth, cTileWidth});

    auto size = zones.size();

    if(showZones)
    {
        int32_t maxZoneId = 0;
        for(int32_t y = 0; y < size.y; ++y)
        {
            for(int32_t x = 0; x < size.x; ++x)
            {
                maxZoneId = std::max(maxZoneId, zones.at({x, y}));
            }
        }

        for(int32_t y = 0; y < size.y; ++y)
        {
            for(int32_t x = 0; x < size.x; ++x)
            {
                int32_t id = zones.at({x, y});
                if(id)
                {
                    quad.setPosition(glm::vec2(x, y) * 32.0f);
                    quad.setColor(fea::HSVColor(id / static_cast<float>(maxZoneId), 0.6f, 0.6f).toRGB());
                    mRenderer.render(quad);
                }
            }
        }
    }

    size = walls.size();
    const auto& hWallArray = walls.horizontalWalls();

    quad = fea::Quad({cTileWidth, cWallThickness});
    quad.setColor(fea::Color(90, 90, 90));

    for(int32_t y = 0; y < size.y; ++y)
    {
        for(int32_t x = 0; x < size.x; ++x)
        {
            if(hWallArray[walls.toIndex({x, y})])
            {
                quad.setPosition(glm::vec2(x, y) * 32.0f - glm::vec2(0.0f, cWallThickness / 2.0f));
                mRenderer.render(quad);
            }
        }
    }

    const auto& vWallArray = walls.verticalWalls();

    quad.setSize({cWallThickness, cTileWidth});

    for(int32_t y = 0; y < size.y; ++y)
    {
        for(int32_t x = 0; x < size.x; ++x)
        {
            if(vWallArray[walls.toIndex({x, y})])
            {
                quad.setPosition(glm::vec2(x, y) * 32.0f - glm::vec2(cWallThickness / 2.0f, 0.0f));
                mRenderer.render(quad);
            }
        }
    }


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
        const Texture* texture = nullptr;
        th::Optional<glm::vec2> size;
        if(renderOrder.textureId)
        {
            const auto textureIter = std::find_if(mTextures.begin(), mTextures.end(), [&renderOrder] (const Texture& disp) { return disp.textureId == *renderOrder.textureId; });
            TH_ASSERT(textureIter != mTextures.end(), "render order uses invalid texture " << *renderOrder.textureId);
            texture = &*textureIter;
            size = texture->size;
        }

        if(renderOrder.size)
            size = renderOrder.size;

        TH_ASSERT(size, "no size supplied for render order");


        if(renderOrder.fillType == FillType::Solid)
        {
            fea::AnimatedQuad quad(*size);

            if(texture && texture->texture)
                quad.setTexture(*texture->texture);

            quad.setPosition(renderOrder.position);// + offset);
            //quad.setRotation(renderOrder.rotation - pi / 2.0f); //take away pi/2.0f which is a quarter of a full turn because featherkit expects 0 degrees to be --> but in the game, it is downwards
            //quad.setHFlip(renderOrder.flip);
            quad.setColor(renderOrder.color);

            mRenderer.render(quad);
        }
        else if(renderOrder.fillType == FillType::Hollow)
        {
            LineRect lineRect(*size);

            lineRect.setPosition(renderOrder.position);
            lineRect.setColor(renderOrder.color);

            mRenderer.render(lineRect);
        }
    }

    DRen::flush();
}
