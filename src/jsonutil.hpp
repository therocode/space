#pragma once
#include <json.hpp>
#include <string>
#include <thero/optional.hpp>
#include <fea/rendering/color.hpp>
#include "glm.hpp"

th::Optional<nlohmann::json> extractOptional(const nlohmann::json& json, const std::string& attribute);

template<typename T>
T extractOrDefault(const nlohmann::json& json, const std::string& attribute, T defaultValue)
{
    if(json.find(attribute) != json.end())
    {
        if(json[attribute] != nullptr)
            return json[attribute].get<T>();
    }
    return defaultValue;
}

glm::vec2 extractVec2(const nlohmann::json& json, const std::string& attribute);
glm::vec2 extractVec2(const nlohmann::json& json, const std::string& attribute, glm::vec2 defaultVal);

fea::Color extractColor(const nlohmann::json& json, const std::string& attribute);
fea::Color extractColor(const nlohmann::json& json, const std::string& attribute, glm::vec2 defaultVal);
