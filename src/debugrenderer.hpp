#pragma once
#include <fea/render2d.hpp>
#include "glm.hpp"

struct DLine
{
    glm::vec2 start;
    glm::vec2 end;
    fea::Color color = {255, 255, 255, 200};
};

struct DRect
{
    glm::vec2 start;
    glm::vec2 size;
    fea::Color color = {255, 255, 255, 200};
};

struct DFrame
{
    glm::vec2 start;
    glm::vec2 size;
    fea::Color color = {255, 255, 255, 200};
};

struct DPixels
{
    struct PixelInfo
    {
        glm::ivec2 coordinate;
        fea::Color color = {255, 255, 255, 200};
    };

    int32_t pixelSize = 1;
    std::vector<PixelInfo> pixels = {};
};

struct DMark
{
    glm::vec2 position;
    fea::Color color = {255, 255, 255, 200};
    float size = 16.0f;
};

class DRen
{
    enum class DebugType { LINE, RECTANGLE, FRAME, PIXELS, MARK };

    public:
        static void initialize(fea::Renderer2D& renderer, const fea::Camera& camera);
        static void setCamera(const fea::Camera& camera);
        static void flush();
        static void queue(DLine line);
        static void queue(DRect rectangle);
        static void queue(DFrame frame);
        static void queue(DPixels pixels);
        static void queue(DMark marks);

        static DRen out;
    private:
        static fea::Renderer2D* mRenderer;
        static fea::Texture mPixelTexture;
        static fea::Camera mCamera;
        static std::vector<DebugType> mTypes;
        static std::vector<DLine> mLines;
        static std::vector<DRect> mRectangles;
        static std::vector<DFrame> mFrames;
        static std::vector<DPixels> mPixels;
        static std::vector<DMark> mMarks;
};

DRen& operator<<(DRen& dren, DLine line);
DRen& operator<<(DRen& dren, DRect rectangle);
DRen& operator<<(DRen& dren, DFrame frame);
DRen& operator<<(DRen& dren, DPixels pixels);
DRen& operator<<(DRen& dren, DMark marks);
template<typename T>
DRen& operator<<(DRen& dren, const std::vector<T>& debugs)
{
    for(const T& debug : debugs)
        dren << debug;

    return dren;
}

//Project specific:

template<typename DRenderable>
struct TTLDRenderable
{
    DRenderable renderable;
    int32_t ttl;
    bool showTTL;
};

template<typename DRenderable>
DRen& operator<<(DRen& dren, std::vector<TTLDRenderable<DRenderable>>& debugs)
{
    for(TTLDRenderable<DRenderable>& debug : debugs)
        dren << debug;

    return dren;
}

template<typename DRenderable>
void clearExpired(std::vector<TTLDRenderable<DRenderable>>& ttlInstances)
{
    for(auto iter = ttlInstances.begin(); iter != ttlInstances.end();)
    {
        if(iter->ttl <= 0)
            iter = ttlInstances.erase(iter);
        else
            ++iter;
    }
}

template<typename DRenderable>
DRen& operator<<(DRen& dren, TTLDRenderable<DRenderable>& ttlInstance)
{
    if(ttlInstance.showTTL)
    {
        float fadePercent = std::min(std::max(0.0f, ttlInstance.ttl / 120.0f), 1.0f);
        ttlInstance.renderable.color.setAAsFloat(fadePercent);
    }
    dren.queue(ttlInstance.renderable);
    --ttlInstance.ttl;

    return dren;
}

using TTLFrame = TTLDRenderable<DFrame>;
using TTLLine = TTLDRenderable<DLine>;
