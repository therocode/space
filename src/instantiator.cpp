#include "instantiator.hpp"
#include <fstream>
#include "jsonutil.hpp"

Instantiator::Instantiator(const ResourceManager& resources)
{
    std::ifstream templateFile("data/templates.json");
    nlohmann::json templateJson;
    templateFile >> templateJson;

    for(const auto& templateEntry : templateJson)
    {
        Object templateToStore;

        std::string name = templateEntry["name"];

        auto position = extractOptional(templateEntry, "position");

        if(position)
        {
            templateToStore.position = extractVec2(*position, "position", {0.0f, 0.0f});
        }

        auto physics = extractOptional(templateEntry, "physics");

        //if(physics)
        //{
        //    templateToStore.physics = Physics
        //    {
        //        extractVec2(*physics, "velocity", {0.0f, 0.0f}),
        //        extractVec2(*physics, "acceleration", {0.0f, 0.0f}),
        //    };            
        //}

        //auto walkSpeed = extractOptional(templateEntry, "walk_speed");

        //if(walkSpeed)
        //{
        //    templateToStore.walkSpeed = WalkSpeed
        //    {
        //        walkSpeed->get<float>(),
        //    };            
        //}

        //auto collisionBox = extractOptional(templateEntry, "collision_box");

        //if(collisionBox)
        //{
        //    templateToStore.collisionBox = CollisionBox
        //    {
        //        extractVec2(*collisionBox, "start"),
        //        extractVec2(*collisionBox, "size"),
        //    };
        //}

        auto sprites = extractOptional(templateEntry, "sprites");

        if(sprites)
        {
            for(const auto& sprite : *sprites)
            {
                templateToStore.actorSprites.push_back(ActorSprite
                {
                    0,
                    resources.textureFromName(sprite["sprite"]),
                    extractColor(sprite, "color"),
                });
            }
        }

        int32_t newId = mIdPool.next();
        mTemplates.emplace(newId, std::move(templateToStore));
        mTemplateNames.emplace(name, newId);
    }   
}

Object Instantiator::instantiate(int32_t instanceId, int32_t objectId, const glm::vec2& position)
{
    auto newObject = mTemplates.at(instanceId);
    newObject.id = objectId;

    for(auto& sprite : newObject.actorSprites)
        sprite.actorId = objectId;

    if(newObject.position)
        newObject.position = position;

    return newObject;
}

Object Instantiator::instantiate(const std::string& name, int32_t objectId, const glm::vec2& position)
{
    return instantiate(mTemplateNames.at(name), objectId, position);
}

int32_t Instantiator::id(const std::string& name)
{
    return mTemplateNames.at(name);
}
