#pragma once
#include <fea/rendering/drawable2d.hpp>

class Line : public fea::Drawable2D
{
    public:
        Line();
        Line(glm::vec2 start, glm::vec2 end);
        void updateRenderInfo(std::vector<fea::RenderEntity>& renderInfo, bool updateVertices, bool updateUniforms) const override;
    protected:
        glm::vec2 mStart;
        glm::vec2 mEnd;
};
