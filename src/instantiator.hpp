#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "object.hpp"
#include "util/numberpool.hpp"
#include "resourcemanager.hpp"

class Instantiator
{
    public:
        Instantiator(const ResourceManager& resources);
        Object instantiate(int32_t instanceId, int32_t objectId, const glm::vec2& position = glm::vec2());
        Object instantiate(const std::string& name, int32_t objectId, const glm::vec2& position = glm::vec2());
        int32_t id(const std::string& name);
    private:
        std::unordered_map<int32_t, Object> mTemplates; 
        std::unordered_map<std::string, int32_t> mTemplateNames; 
        NumberPool<int32_t> mIdPool;
};
