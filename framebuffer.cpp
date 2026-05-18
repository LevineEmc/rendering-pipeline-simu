#include "framebuffer.h"

#include <fstream>
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
FrameBuffer::FrameBuffer(int width, int height)
{
    this->width = width;
    this->height = height;

    pixels.resize(width * height);
    depthBuffer.resize(width * height);
}

void FrameBuffer::clear(Color color)
{
    for (int i = 0; i < pixels.size(); i++)
    {
        pixels[i] = color;

        depthBuffer[i] = 1e9;
    }
}

void FrameBuffer::setPixel(int x, int y, Color color)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height)
    {
        return;
    }

    pixels[y * width + x] = color;
}

void FrameBuffer::drawLine(int x0,int y0,int x1,int y1,Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = std::max(abs(dx),abs(dy));

    float xIncrement = dx / (float)steps;

    float yIncrement = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        setPixel((int)x,(int)y,color);

        x += xIncrement;
        y += yIncrement;
    }
}

static float edgeFunction(float ax, float ay,float bx, float by,float px, float py)
{
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}
void FrameBuffer::drawTriangle(float x0, float y0, float z0,float x1, float y1, float z1 ,float x2, float y2 , float z2, float i0,float i1,float i2)
{
    int minX = std::min(x0,std::min(x1,x2));
    int maxX = std::max(x0, std::max(x1, x2));

    int minY = std::min(y0, std::min(y1, y2));
    int maxY = std::max(y0, std::max(y1, y2));

    float area = edgeFunction(
    x0, y0,
    x1, y1,
    x2, y2
);

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            float w0 = edgeFunction(
                x1, y1,
                x2, y2,
                x, y
            );

            float w1 = edgeFunction(
                x2, y2,
                x0, y0,
                x, y
            );

            float w2 = edgeFunction(
                x0, y0,
                x1, y1,
                x, y
            );

            bool hasNeg =(w0 < -0.001f) ||(w1 < -0.001f) ||(w2 < -0.001f);

            bool hasPos =(w0 > 0.001f) ||(w1 > 0.001f) ||(w2 > 0.001f);

            if (!(hasNeg && hasPos))
            {
            // if (w0 >= 0 &&w1 >= 0 &&w2 >= 0)
            // {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float depth =w0 * z0 + w1 * z1 + w2 * z2;
                float intensity =w0 * i0 +w1 * i1 +w2 * i2;
                unsigned char c =(unsigned char)(intensity * 255);
                setPixelDepth(x, y, depth,{c,c,c});
                //setPixel(x, y, );
            }
        }
    }
}

void FrameBuffer::setPixelDepth(int x,int y,float depth,Color color)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height)
    {
        return;
    }

    int index = y * width + x;
    //z-buffer depth test
    if (depth < depthBuffer[index])
    {
        depthBuffer[index] = depth;

        pixels[index] = color;
    }
}



void FrameBuffer::savePPM(const std::string& filename)
{
    std::ofstream out(filename);

    out << "P3\n";
    out << width << " " << height << "\n";
    out << 255 << "\n";

    for (const auto& p : pixels)
    {
        out << (int)p.r << " "
            << (int)p.g << " "
            << (int)p.b << "\n";
    }

    out.close();
}
void FrameBuffer::savePNG(const std::string& filename)
{
    stbi_write_png(
        filename.c_str(),
        width,
        height,
        3,
        pixels.data(),
        width * 3
    );
}