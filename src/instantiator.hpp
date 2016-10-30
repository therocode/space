#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "actor.hpp"
#include <numberpool.hpp>
#include "resourcemanager.hpp"

class Instantiator
{
    public:
        Instantiator(const ResourceManager& resources);
        Actor instantiate(int32_t instanceId, const glm::vec2& position = glm::vec2());
        Actor instantiate(const std::string& name, const glm::vec2& position = glm::vec2());
        int32_t id(const std::string& name);
    private:
        std::unordered_map<int32_t, Actor> mTemplates; 
        std::unordered_map<std::string, int32_t> mTemplateNames; 
        NumberPool<int32_t> mIdPool;
};
