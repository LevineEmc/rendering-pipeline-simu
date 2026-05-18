#include "framebuffer.h"
#include "objloader.h"
#include "mat4.h"

#include <algorithm>
#include <cmath>

int main()
{
    // framebuffer
    const int WIDTH = 800;
    const int HEIGHT = 600;

    FrameBuffer fb(WIDTH, HEIGHT);

    fb.clear({30,30,30});
    // load obj
    OBJLoader obj;
    if (!obj.load("assets/teapot.obj"))
    {
        return -1;
    }

    // light
    Vec3 lightDir =
        Vec3(0, 0, 1).normalize();


    // MVP
    // 模型旋转
    Mat4 model =
        Mat4::rotation(
            0.5f,
             -0.5f,
             0.0f
        );

    // 相机后退
    Mat4 view =
        Mat4::translation(
            0,
           -20,
           -150
        );

    // 透视矩阵
    Mat4 projection =
        Mat4::perspective(
            M_PI / 3.0f,
            (float)WIDTH / HEIGHT,
            0.1f,
            1000.0f
        );

    // render

    for (const auto& face : obj.faces)
    {


        if (face.v0 < 0 || face.v0 >= obj.vertices.size() ||
            face.v1 < 0 || face.v1 >= obj.vertices.size() ||
            face.v2 < 0 || face.v2 >= obj.vertices.size())
        {
            continue;
        }

        // original vertices


        Vec3 v0 = obj.vertices[face.v0];
        Vec3 v1 = obj.vertices[face.v1];
        Vec3 v2 = obj.vertices[face.v2];

        Vec4 p0(v0.x, v0.y, v0.z, 1);
        Vec4 p1(v1.x, v1.y, v1.z, 1);
        Vec4 p2(v2.x, v2.y, v2.z, 1);

        // MVP transform


        p0 = projection * view * model * p0;
        p1 = projection * view * model * p1;
        p2 = projection * view * model * p2;

        // perspective divide

        if (fabs(p0.w) < 0.0001f ||
            fabs(p1.w) < 0.0001f ||
            fabs(p2.w) < 0.0001f)
        {
            continue;
        }

        p0.x /= p0.w;
        p0.y /= p0.w;
        p0.z /= p0.w;

        p1.x /= p1.w;
        p1.y /= p1.w;
        p1.z /= p1.w;

        p2.x /= p2.w;
        p2.y /= p2.w;
        p2.z /= p2.w;

        // clip


        if (p0.z < -1 || p0.z > 1 ||
            p1.z < -1 || p1.z > 1 ||
            p2.z < -1 || p2.z > 1)
        {
            continue;
        }

        // viewport transform

        int x0 =
            (p0.x + 1.0f) * 0.5f * WIDTH;

        int y0 =
            (1.0f - p0.y) * 0.5f * HEIGHT;

        int x1 =
            (p1.x + 1.0f) * 0.5f * WIDTH;

        int y1 =
            (1.0f - p1.y) * 0.5f * HEIGHT;

        int x2 =
            (p2.x + 1.0f) * 0.5f * WIDTH;

        int y2 =
            (1.0f - p2.y) * 0.5f * HEIGHT;


        // normals


        Vec3 n0 = obj.normals[face.v0];
        Vec3 n1 = obj.normals[face.v1];
        Vec3 n2 = obj.normals[face.v2];

        float i0 =
            std::max(0.0f,
            n0.dot(lightDir));

        float i1 =
            std::max(0.0f,
            n1.dot(lightDir));

        float i2 =
            std::max(0.0f,
            n2.dot(lightDir));

        // rasterize


        fb.drawTriangle(
            x0, y0, p0.z,
            x1, y1, p1.z,
            x2, y2, p2.z,
            i0, i1, i2
        );
    }

    // save
    fb.savePNG("output.png");

    return 0;
}