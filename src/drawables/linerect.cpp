#include "linerect.hpp"
#include <thero/assert.hpp>

LineRect::LineRect() : LineRect(glm::vec2(0.0f, 0.0f))
{

}

LineRect::LineRect(glm::vec2 size):
    mSize(std::move(size))
{
    mVertices =
    {
        0.0f, 0.0f, 
        mSize.x, 0.0f,
        mSize.x, mSize.y,
        0.0f, mSize.y,
        0.0f, 0.0f, 
    };

    mTexCoords =
    {
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
    };

    mVertexColors =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    mDrawMode = GL_LINE_STRIP;

    mVerticesDirty = true;
}

void LineRect::updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const
{
    Drawable2D::updateRenderInfo(renderInfo, updateVertices, updateUniforms);
    fea::RenderEntity& renderEntity = renderInfo.front();

    renderEntity.mElementAmount = 5; //this could be worked out correctly from drawmode. it must now be set in the child
}
