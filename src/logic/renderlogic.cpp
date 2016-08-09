#include "renderlogic.hpp"
#include "../debug.hpp"
#include "../drawables/linerect.hpp"

RenderLogic::RenderLogic(ResourceManager& resources, fea::Renderer2D& feaRenderer, const WallMap& walls, const gfx::TActorSprite& tActorSprite, const ent::TPosition& tPosition, const tsk::TRoomTask& tRoomTask):
    mResources(resources),
    mFeaRenderer(feaRenderer),
    mRenderer(mFeaRenderer, mResources.textures()),
    mWalls(walls),
    mTActorSprite(tActorSprite),
    mTPosition(tPosition),
    mTRoomTask(tRoomTask)
{
}

void RenderLogic::frameStart()
{
    ImGui::NewFrame();
}

void RenderLogic::update()
{
    mRenderer.startFrame();
    mRenderer.renderWorld(mWalls);
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
                {}
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
                //sprite.rotation,
                //sprite.animationProgress,
                //sprite.flip,
                }   
                );  
    }, mTRoomTask); 

    mRenderer.render(orders);
}
