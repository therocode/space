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
        RenderLogic(ResourceManager& resources, fea::Renderer2D& feaRenderer, const WallMap& walls, const Zones& zones, const Grid<Gases>& atmosphere, const GfxData& gfx, const EntityData& ent, const StructureData& str, const TaskData& tsk, const WorldData& wld, bool& showZones, bool& showAtmosphere);
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

        const WallMap& mWalls;
        const Zones& mZones;
        const Grid<Gases>& mAtmosphere;

        const GfxData& mGfx;
        const EntityData& mEnt;
        const StructureData& mStr;
        const TaskData& mTsk;
        const WorldData& mWld;
        bool& mShowZones;
        bool& mShowAtmosphere;
};
