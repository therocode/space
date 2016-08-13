#include "renderlogic.hpp"
#include "../debug.hpp"
#include "../drawables/linerect.hpp"

RenderLogic::RenderLogic(ResourceManager& resources, fea::Renderer2D& feaRenderer, const WallMap& walls, const Zones& zones, const gfx::TActorSprite& tActorSprite, const ent::TPosition& tPosition, const tsk::TRoomTask& tRoomTask, const tsk::TWallTask& tWallTask, bool& showZones):
    mResources(resources),
    mFeaRenderer(feaRenderer),
    mRenderer(mFeaRenderer, mResources.textures()),
    mWalls(walls),
    mZones(zones),
    mTActorSprite(tActorSprite),
    mTPosition(tPosition),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask),
    mShowZones(showZones)
{
}

void RenderLogic::frameStart()
{
    ImGui::NewFrame();
}

void RenderLogic::update()
{
    mRenderer.startFrame();
    mRenderer.renderWorld(mWalls, mZones.zones, mShowZones);
    renderTasks();
    renderSprites();
}

void RenderLogic::frameEnd()
{
    ImGui::Render();
    mRenderer.renderImGui(*ImGui::GetDrawData());
}

void RenderLogic::renderSprites()
{
    std::vector<RenderOrder> orders;

     forEach([&] (int32_t actorId, const ActorSprite& sprite)
    {
        const glm::vec2& position = get(sprite.actorId, mTPosition).data;

        orders.emplace_back(
                RenderOrder{
                position,
                sprite.textureId,
                sprite.color,
                {},
                FillType::Solid,
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  

        if(dbg::exists("selected_actor") && dbg::get<int32_t>("selected_actor") == actorId)
        {
            const auto textureIter = std::find_if(mResources.textures().begin(), mResources.textures().end(), [&] (const Texture& disp) { return disp.textureId == sprite.textureId; });

            LineRect rect(textureIter->size);
            rect.setPosition(position);
            rect.setColor(fea::Color::Yellow);
            mFeaRenderer.render(rect);
        }
    }, mTActorSprite); 

    mRenderer.render(orders);
}

void RenderLogic::renderTasks()
{
    std::vector<RenderOrder> orders;

    forEach([&] (int32_t taskId, const RoomTask& roomTask)
    {   
        const glm::vec2& position = roomTask.position * 32;
        const glm::vec2& size = roomTask.size * 32;

        orders.emplace_back(
                RenderOrder{
                position,
                {},
                fea::Color(50, 150, 0, 200),
                size,
                FillType::Hollow,
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  
    }, mTRoomTask); 

    forEach([&] (int32_t taskId, const WallTask& wallTask)
    {   
        Orientation orientation = wallTask.orientation;
        const glm::vec2& position = static_cast<glm::vec2>(wallTask.position * 32) - 
            (orientation == Orientation::Horizontal ? glm::vec2{0.0f, 4.0f} : glm::vec2{4.0f, 0.0f});

        glm::vec2 size = orientation == Orientation::Horizontal ? glm::vec2{32.0f, 8.0f} : glm::vec2{8.0f, 32.0f};

        orders.emplace_back(
                RenderOrder{
                position,
                {},
                fea::Color(50, 150, 0, 200),
                size,
                FillType::Hollow,
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  
    }, mTWallTask); 

    mRenderer.render(orders);
}
