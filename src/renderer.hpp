#pragma once
#include <fea/render2d.hpp>
#include "renderdata.hpp"

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer, const std::vector<Texture>& textures);
        void startFrame();
        void renderWorld();
        void render(const std::vector<RenderOrder>& orders) const;
    private:
        fea::Renderer2D& mRenderer;
        
        const std::vector<Texture>& mTextures;
};
