#include "line.hpp"
#include <thero/assert.hpp>

Line::Line() : Line(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
{

}

Line::Line(glm::vec2 start, glm::vec2 end):
    mStart(std::move(start)),
    mEnd(std::move(end))
{
    mVertices = {mStart.x, mStart.y, 
        mEnd.x, mEnd.y};

    mTexCoords =  {0.0f, 0.0f,
        0.0f, 0.0f};

    mVertexColors = {1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    mDrawMode = GL_LINES;

    mVerticesDirty = true;
}

void Line::updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const
{
    Drawable2D::updateRenderInfo(renderInfo, updateVertices, updateUniforms);
    fea::RenderEntity& renderEntity = renderInfo.front();

    renderEntity.mElementAmount = 2; //this could be worked out correctly from drawmode. it must now be set in the child
}
