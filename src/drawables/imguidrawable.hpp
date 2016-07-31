#pragma once
#include <fea/rendering/drawable2d.hpp>
#include <imgui.h>

class ImGuiDrawable : public fea::Drawable2D
{
    public:
        ImGuiDrawable(const unsigned char* vertexBuffer, const ImDrawIdx* indexBuffer, int32_t elementCount, void* textureId); 
    protected:
        void updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const;

        std::vector<uint16_t> mIndices;
        GLuint mTextureId;
};
