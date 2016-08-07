#pragma once
#include <fea/rendering/drawable2d.hpp>

class LineRect : public fea::Drawable2D
{
    public:
        LineRect();
        LineRect(glm::vec2 size);
        void updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const override;
    protected:
        glm::vec2 mSize;
};
