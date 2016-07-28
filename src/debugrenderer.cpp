#include "debugrenderer.hpp"
#include "drawables/line.hpp"
#include <algorithm>

void DRen::initialize(fea::Renderer2D& renderer, const fea::Camera& camera)
{
    mRenderer = &renderer;
    mCamera = camera;
    mPixelTexture.create({64, 64}, fea::Color::Transparent);
}

void DRen::setCamera(const fea::Camera& camera)
{
    mCamera = camera;
}

void DRen::flush()
{
    fea::Camera oldCam = mRenderer->getViewport().getCamera();
    mRenderer->getViewport().setCamera(mCamera);

    auto nextLine = mLines.begin();
    auto nextRectangle = mRectangles.begin();
    auto nextFrame = mFrames.begin();
    auto nextPixels = mPixels.begin();
    auto nextMark = mMarks.begin();

    for(DebugType nextType : mTypes)
    {
        if(nextType == DebugType::LINE)
        {
            const DLine& lineData = *nextLine;
            Line line(lineData.start, lineData.end);
            line.setColor(lineData.color);
            mRenderer->render(line);

            ++nextLine;
        }
        else if(nextType == DebugType::RECTANGLE)
        {
            const DRect& rectangleData = *nextRectangle;
            fea::Quad quad(rectangleData.size);

            quad.setPosition(rectangleData.start);
            quad.setColor(rectangleData.color);
            mRenderer->render(quad);

            ++nextRectangle;
        }
        else if(nextType == DebugType::FRAME)
        {
            const DFrame& frameData = *nextFrame;

            Line line(frameData.start, frameData.start + glm::vec2(0.0f, frameData.size.y));
            line.setColor(frameData.color);
            mRenderer->render(line);
            line = Line(frameData.start, frameData.start + glm::vec2(frameData.size.x, 0.0f));
            line.setColor(frameData.color);
            mRenderer->render(line);
            line = Line(frameData.start + glm::vec2(frameData.size.x, 0.0f), frameData.start + glm::vec2(frameData.size.x, frameData.size.y));
            line.setColor(frameData.color);
            mRenderer->render(line);
            line = Line(frameData.start + glm::vec2(0.0f, frameData.size.y), frameData.start + glm::vec2(frameData.size.x, frameData.size.y));
            line.setColor(frameData.color);
            mRenderer->render(line);

            ++nextFrame;
        }
        else if(nextType == DebugType::PIXELS)
        {
            DPixels pixelData = std::move(*nextPixels);

            while(!pixelData.pixels.empty())
            {
                for(int32_t y = 0; y < mPixelTexture.getSize().y; ++y)
                {
                    for(int32_t x = 0; x < mPixelTexture.getSize().x; ++x)
                    {
                        mPixelTexture.setPixel({x, y}, fea::Color::Transparent);
                    }
                }

                const DPixels::PixelInfo& topLeft = *std::min_element(pixelData.pixels.begin(), pixelData.pixels.end(), [] (const DPixels::PixelInfo& a, const DPixels::PixelInfo& b)
                {
                        return a.coordinate.x == b.coordinate.x ? a.coordinate.y < b.coordinate.y : a.coordinate.x < b.coordinate.x;
                });
                
                glm::ivec2 subRegionStart = topLeft.coordinate;
                glm::ivec2 subRegionEnd = subRegionStart + mPixelTexture.getSize();

                for(auto iter = pixelData.pixels.begin(); iter != pixelData.pixels.end();)
                {
                    const DPixels::PixelInfo& pixel = *iter;
                    if(pixel.coordinate.x >= subRegionStart.x && pixel.coordinate.x < subRegionEnd.x &&
                            pixel.coordinate.y >= subRegionStart.y && pixel.coordinate.y < subRegionEnd.y)
                    {
                        mPixelTexture.setPixel(pixel.coordinate - subRegionStart, pixel.color);
                        *iter = pixelData.pixels.back();
                        pixelData.pixels.pop_back();
                    }
                    else
                    {
                        ++iter;
                    }
                }

                mPixelTexture.update();

                fea::Quad pixelQuad(static_cast<glm::vec2>(mPixelTexture.getSize() * pixelData.pixelSize));
                pixelQuad.setTexture(mPixelTexture);
                pixelQuad.setPosition(static_cast<glm::vec2>(subRegionStart * pixelData.pixelSize));

                mRenderer->render(pixelQuad);
            }

            ++nextPixels;
        }
        else if(nextType == DebugType::MARK)
        {
            const DMark& markData = *nextMark;
            float halfSize = markData.size / 2.0f;
            Line line(markData.position + glm::vec2(-halfSize, -halfSize), markData.position + glm::vec2(halfSize, halfSize));
            line.setColor(markData.color);
            mRenderer->render(line);
            line = Line(markData.position + glm::vec2(halfSize, -halfSize), markData.position + glm::vec2(-halfSize, halfSize));
            line.setColor(markData.color);
            mRenderer->render(line);
            ++nextMark;
        }
    }

    mTypes.clear();
    mLines.clear();
    mRectangles.clear();
    mFrames.clear();
    mPixels.clear();
    mMarks.clear();
    mRenderer->getViewport().setCamera(oldCam);
}

void DRen::queue(DLine line)
{
    mTypes.emplace_back(DebugType::LINE);
    mLines.emplace_back(std::move(line));
}

void DRen::queue(DRect rectangle)
{
    mTypes.emplace_back(DebugType::RECTANGLE);
    mRectangles.emplace_back(std::move(rectangle));
}

void DRen::queue(DFrame frame)
{
    mTypes.emplace_back(DebugType::FRAME);
    mFrames.emplace_back(std::move(frame));
}

void DRen::queue(DPixels pixels)
{
    mTypes.emplace_back(DebugType::PIXELS);
    mPixels.emplace_back(std::move(pixels));
}

void DRen::queue(DMark mark)
{
    mTypes.emplace_back(DebugType::MARK);
    mMarks.emplace_back(std::move(mark));
}

DRen DRen::out = DRen();
fea::Renderer2D* DRen::mRenderer = nullptr;
fea::Texture DRen::mPixelTexture = fea::Texture();
fea::Camera DRen::mCamera = {};
std::vector<DRen::DebugType> DRen::mTypes = {};
std::vector<DLine> DRen::mLines = {};
std::vector<DRect> DRen::mRectangles = {};
std::vector<DFrame> DRen::mFrames = {};
std::vector<DPixels> DRen::mPixels = {};
std::vector<DMark> DRen::mMarks = {};

DRen& operator<<(DRen& dren, DLine line)
{
    dren.queue(std::move(line));
    return dren;
}

DRen& operator<<(DRen& dren, DRect rectangle)
{
    dren.queue(std::move(rectangle));
    return dren;
}

DRen& operator<<(DRen& dren, DFrame frame)
{
    dren.queue(std::move(frame));
    return dren;
}

DRen& operator<<(DRen& dren, DPixels pixels)
{
    dren.queue(std::move(pixels));
    return dren;
}

DRen& operator<<(DRen& dren, DMark mark)
{
    dren.queue(std::move(mark));
    return dren;
}

//Project specific:
