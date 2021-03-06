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
        Actor templateToStore;

        std::string name = templateEntry["name"];

        auto position = extractOptional(templateEntry, "position");

        if(position)
        {
            templateToStore.position = Position{extractVec2(*position, "position", {0.0f, 0.0f})};
        }

        auto physics = extractOptional(templateEntry, "physics");

        if(physics)
        {
            templateToStore.physics = Physics
            {
                extractVec2(*physics, "velocity", {0.0f, 0.0f}),
                extractVec2(*physics, "acceleration", {0.0f, 0.0f}),
            };            
        }

        auto moveAbility = extractOptional(templateEntry, "move_ability");

        if(moveAbility)
        {
            templateToStore.moveAbility = MoveAbility
            {
                (*moveAbility)["max_speed"].get<float>(),
                (*moveAbility)["max_acceleration"].get<float>(),
            };
        }

        auto bloodValues = extractOptional(templateEntry, "blood_values");

        if(bloodValues)
        {
            templateToStore.bloodValues = BloodValues{(*bloodValues)["oxygen"]};
        }

        templateToStore.worker = templateEntry["worker"];
        std::string ai = templateEntry["ai"];
        if(ai == "human")
            templateToStore.ai = AiType::Human;

        //auto walkSpeed = extractOptional(templateEntry, "walk_speed");

        //if(walkSpeed)
        //{
        //    templateToStore.walkSpeed = WalkSpeed
        //    {
        //        walkSpeed->get<float>(),
        //    };            
        //}

        auto collisionBox = extractOptional(templateEntry, "collision_box");

        if(collisionBox)
        {
            templateToStore.collisionBox = CollisionBox
            {
                //extractVec2(*collisionBox, "start"),
                extractVec2(*collisionBox, "size"),
            };
        }

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

Actor Instantiator::instantiate(int32_t instanceId, const glm::vec2& position)
{
    auto newActor = mTemplates.at(instanceId);

    newActor.position = Position{position};

    return newActor;
}

Actor Instantiator::instantiate(const std::string& name, const glm::vec2& position)
{
    return instantiate(mTemplateNames.at(name), position);
}

int32_t Instantiator::id(const std::string& name)
{
    return mTemplateNames.at(name);
}
