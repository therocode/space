#pragma once
#include "../renderer.hpp"
#include <fea/render2d.hpp>
#include "../resourcemanager.hpp"
#include "../wallmap.hpp"
#include "../zones.hpp"
#include "../gases.hpp"
#include <data.hpp>

class RenderLogic
{
    public:
        RenderLogic(ResourceManager& resources, fea::Renderer2D& feaRenderer, const GameData& data, bool& showZones, bool& showAtmosphere);
        void frameStart();
        void update();
        void frameEnd();
    private:
        void renderSprites();
        void renderStructures();
        void renderTasks();
        ResourceManager& mResources;
        fea::Renderer2D& mFeaRenderer;
        Renderer mRenderer;

        const GameData& mData;

        bool& mShowZones;
        bool& mShowAtmosphere;
};
