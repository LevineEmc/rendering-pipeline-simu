

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#pragma once

#include <vector>
#include <string>

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class FrameBuffer
{
public:

    FrameBuffer(int width, int height);

    void clear(Color color);

    void setPixel(int x, int y, Color color);
    //画线 DDA
    void drawLine(int x0,int y0,int x1,int y1,Color color);
    //画三角形
    void drawTriangle(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float i0, float i1, float i2
);

    void setPixelDepth(int x,int y,float depth,Color color);
    void savePPM(const std::string& filename);
    void savePNG(const std::string& filename);

private:

    int width;
    int height;

    std::vector<Color> pixels;
    std::vector<float> depthBuffer;
};

#endif //FRAMEBUFFER_H
