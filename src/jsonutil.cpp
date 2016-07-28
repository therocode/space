#include "jsonutil.hpp"

th::Optional<nlohmann::json> extractOptional(const nlohmann::json& json, const std::string& attribute)
{
    if(json.find(attribute) != json.end())
    {
        if(json[attribute] != nullptr)
        {
            return {json[attribute]};
        }
    }
    return {};
}

glm::vec2 extractVec2(const nlohmann::json& json, const std::string& attribute)
{
    glm::vec2 result;

    auto vec2 = json[attribute];

    if(vec2.find("x") != vec2.end())
        result.x = vec2["x"];
    if(vec2.find("y") != vec2.end())
        result.y = vec2["y"];

    return result;
}

glm::vec2 extractVec2(const nlohmann::json& json, const std::string& attribute, glm::vec2 defaultVal)
{
    glm::vec2 result = std::move(defaultVal);

    if(json.find(attribute) != json.end())
    {
        result = extractVec2(json, attribute);
    }

    return result;
}

fea::Color extractColor(const nlohmann::json& json, const std::string& attribute)
{
    fea::Color result;

    auto color = json[attribute];

    if(color.find("r") != color.end())
        result.r = color["r"];
    if(color.find("g") != color.end())
        result.g = color["g"];
    if(color.find("b") != color.end())
        result.b = color["b"];
    if(color.find("a") != color.end())
        result.a = color["a"];

    return result;
}

fea::Color extractColor(const nlohmann::json& json, const std::string& attribute, fea::Color defaultVal)
{
    fea::Color result = std::move(defaultVal);

    if(json.find(attribute) != json.end())
    {
        result = extractColor(json, attribute);
    }

    return result;
}
