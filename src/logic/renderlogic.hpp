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
        RenderLogic(ResourceManager& resources, fea::Renderer2D& feaRenderer, const WallMap& walls, const Zones& zones, const Grid<Gases>& atmosphere, const gfx::TActorSprite& tActorSprite, const ent::TPosition& tPosition, const tsk::TRoomTask& tRoomTask, const tsk::TWallTask& tWallTask, const ent::TBloodValues& tBloodValues, bool& showZones, bool& showAtmospheres);
        void frameStart();
        void update();
        void frameEnd();
    private:
        void renderSprites();
        void renderTasks();
        ResourceManager& mResources;
        fea::Renderer2D& mFeaRenderer;
        Renderer mRenderer;

        const WallMap& mWalls;
        const Zones& mZones;
        const Grid<Gases>& mAtmosphere;

        const gfx::TActorSprite& mTActorSprite;
        const ent::TPosition& mTPosition;
        const tsk::TRoomTask& mTRoomTask;
        const tsk::TWallTask& mTWallTask;
        const ent::TBloodValues& mTBloodValues;
        bool& mShowZones;
        bool& mShowAtmosphere;
};
