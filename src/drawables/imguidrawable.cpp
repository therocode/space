#include "imguidrawable.hpp"
#include <thero/assert.hpp>

ImGuiDrawable::ImGuiDrawable(const unsigned char* vertexBuffer, const ImDrawIdx* indexBuffer, int32_t elementCount, void* textureId)
{
    mIndices = {indexBuffer, indexBuffer + elementCount};

    mVertices.resize(mIndices.size() * 2);
    mTexCoords.resize(mIndices.size() * 2);
    mVertexColors.resize(mIndices.size() * 4);

    const ImDrawVert* currentPointer = reinterpret_cast<const ImDrawVert*>(vertexBuffer);

    for(size_t i = 0; i < mIndices.size(); ++i)
    {
        size_t index = mIndices[i];
        const ImDrawVert& vertexData = *(currentPointer + index);

        mVertices    [i * 2 + 0] = vertexData.pos.x;
        mVertices    [i * 2 + 1] = vertexData.pos.y;
        mTexCoords   [i * 2 + 0] = vertexData.uv.x;
        mTexCoords   [i * 2 + 1] = vertexData.uv.y;
        const uint8_t* colorPointer = reinterpret_cast<const uint8_t*>(&vertexData.col);
        mVertexColors[i * 4 + 0] = *(colorPointer + 0) / 255.0f;
        mVertexColors[i * 4 + 1] = *(colorPointer + 1) / 255.0f;
        mVertexColors[i * 4 + 2] = *(colorPointer + 2) / 255.0f;
        mVertexColors[i * 4 + 3] = *(colorPointer + 3) / 255.0f;
    }

    mDrawMode = GL_TRIANGLES;
    mTextureId = static_cast<GLuint>(reinterpret_cast<uint64_t>(textureId));
 
    mVerticesDirty = true;
    mUniformsDirty = true;
}

void ImGuiDrawable::updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const
{
    Drawable2D::updateRenderInfo(renderInfo, updateVertices, updateUniforms);
    fea::RenderEntity& renderEntity = renderInfo.front();

    renderEntity.mElementAmount = static_cast<uint32_t>(mIndices.size()); //this could be worked out correctly from drawmode. it must now be set in the child

    if(mUniformsDirty)
    {   
        renderEntity.mUniforms["texture"] = fea::Uniform{fea::TEXTURE, fea::Uniform::TextureData{mTextureId, 0}};
    }   
    if(mVerticesDirty)
    {
        //renderEntity.mIndices = mIndices;
    }
}
