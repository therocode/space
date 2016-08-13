#pragma once
#include <fea/render2d.hpp>
#include "renderdata.hpp"
#include <imgui.h>
#include "wallmap.hpp"
#include "util/grid.hpp"
#include "gases.hpp"

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer, const std::vector<Texture>& textures);
        void startFrame();
        void renderWorld(const WallMap& walls, const Grid<int32_t>& zones, bool showZones, const Grid<Gases>& atmosphere, bool showAtmosphere);
        void renderImGui(ImDrawData& drawData);
        void render(const std::vector<RenderOrder>& orders) const;
    private:
        fea::Renderer2D& mRenderer;
        
        const std::vector<Texture>& mTextures;
};
