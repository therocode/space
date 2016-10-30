#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fea/render2d.hpp>
#include "renderdata.hpp"
#include <numberpool.hpp>

class ResourceManager
{
    public:
        ResourceManager();
        const std::vector<Texture>& textures() const;
        int32_t textureFromName(const std::string& name) const;
    private:
        std::unordered_map<std::string, fea::Texture> mFeaTextures;
        //std::unordered_map<std::string, fea::Animation> mAnimations;
        std::vector<Texture> mTextures;
        std::unordered_map<std::string, int32_t> mTextureNameIndex;
        NumberPool<int32_t> mTextureIdPool;
};
