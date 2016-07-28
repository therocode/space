#include <fstream>
#include <json.hpp>
#include <thero/optional.hpp>
#include "resourcemanager.hpp"
#include "texturemaker.hpp"

ResourceManager::ResourceManager()
{
    std::ifstream texturesFile("data/textures.json");
    nlohmann::json texturesJson;
    texturesFile >> texturesJson;

    for(const auto& textureEntry : texturesJson)
    {
        std::string name = textureEntry["name"];
        std::string path = textureEntry["path"];
        mFeaTextures.emplace(name, makeTexture(path));
    }

    auto feaTexture = [this] (const std::string& name)
    {
        return mFeaTextures.count(name) != 0 ? &mFeaTextures.at(name) : nullptr;
    };

    //std::ifstream animationsFile("data/animations.json");
    //nlohmann::json animationsJson;
    //animationsFile >> animationsJson;

    //for(const auto& animationEntry : animationsJson)
    //{
    //    std::string name = animationEntry["name"];
    //    auto start = animationEntry["start"];
    //    auto size = animationEntry["size"];
    //    int32_t frameCount = animationEntry["frame_count"];
    //    int32_t delay = animationEntry["delay"];

    //    mAnimations.emplace(name, fea::Animation{{start["x"],start["y"]}, {size["x"], size["y"]}, static_cast<uint32_t>(frameCount), static_cast<uint32_t>(delay)});
    //}

    //auto animation = [this] (const std::string& name)
    //{
    //    return mAnimations.count(name) != 0 ? &mAnimations.at(name) : nullptr;
    //};

    auto newTexture = [this] (const std::string& name, Texture texture)
    {
        mTextureNameIndex.emplace(name, texture.textureId);
        mTextures.emplace_back(std::move(texture));
    };

    std::ifstream spritesFile("data/sprites.json");
    nlohmann::json spritesJson;
    spritesFile >> spritesJson;

    for(const auto spriteEntry : spritesJson)
    {
        std::string name = spriteEntry["name"];
        auto offset = spriteEntry["offset"];
        auto size = spriteEntry["size"];
        std::string textureName = spriteEntry["texture"];
        //th::Optional<std::string> animationName = spriteEntry.find("animation") != spriteEntry.end() ? spriteEntry["animation"].get<std::string>() : th::Optional<std::string>();
        //newTexture(name, Texture{mTextureIdPool.next(), {size["x"], size["y"]}, {offset["x"], offset["y"]}, feaTexture(textureName), animationName ? animation(*animationName) : nullptr});
        newTexture(name, Texture{mTextureIdPool.next(), {size["x"], size["y"]}, {offset["x"], offset["y"]}, feaTexture(textureName)});
    }
}

const std::vector<Texture>& ResourceManager::textures() const
{
    return mTextures;
}

int32_t ResourceManager::textureFromName(const std::string& name) const
{
    return mTextureNameIndex.at(name);
}

