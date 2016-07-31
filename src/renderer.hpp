#pragma once
#include <fea/render2d.hpp>
#include "renderdata.hpp"
#include <imgui.h>

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer, const std::vector<Texture>& textures);
        void startFrame();
        void renderWorld();
        void renderImGui(ImDrawData& drawData);
        void render(const std::vector<RenderOrder>& orders) const;
    private:
        fea::Renderer2D& mRenderer;
        
        const std::vector<Texture>& mTextures;
};
