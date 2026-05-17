#include "framebuffer.h"
#include "objloader.h"
#include "mat4.h"

#include <algorithm>
#include <cmath>

int main()
{
    FrameBuffer fb(800, 600);

    fb.clear({30,30,30});

    OBJLoader obj;

    if (!obj.load("assets/teapot.obj"))
    {
        return -1;
    }

    Vec3 lightDir =
        Vec3(0, 0, -1).normalize();

    // =========================
    // MVP
    // =========================

    Mat4 model =
        Mat4::rotation(
            -0.5f,
             0.5f,
             0.0f
        );

    // 相机后退
    Mat4 view =
        Mat4::translation(
            0,
            -45,
            180
        );

    float focalLength = 700.0f;

    for (const auto& face : obj.faces)
    {
        if (face.v0 < 0 || face.v0 >= obj.vertices.size() ||
            face.v1 < 0 || face.v1 >= obj.vertices.size() ||
            face.v2 < 0 || face.v2 >= obj.vertices.size())
        {
            continue;
        }

        // =========================
        // 原始顶点
        // =========================

        Vec3 v0 = obj.vertices[face.v0];
        Vec3 v1 = obj.vertices[face.v1];
        Vec3 v2 = obj.vertices[face.v2];

        // =========================
        // MVP
        // =========================

        Vec4 p0(v0.x, v0.y, v0.z, 1);
        Vec4 p1(v1.x, v1.y, v1.z, 1);
        Vec4 p2(v2.x, v2.y, v2.z, 1);

        p0 = view * model * p0;
        p1 = view * model * p1;
        p2 = view * model * p2;

        v0 = Vec3(p0.x, p0.y, p0.z);
        v1 = Vec3(p1.x, p1.y, p1.z);
        v2 = Vec3(p2.x, p2.y, p2.z);

        // =========================
        // near clip
        // =========================

        if (v0.z <= 0 ||
            v1.z <= 0 ||
            v2.z <= 0)
        {
            continue;
        }

        // =========================
        // perspective
        // =========================

        int x0 =
            (v0.x / v0.z) * focalLength + 400;

        int y0 =
            300 - (v0.y / v0.z) * focalLength;

        int x1 =
            (v1.x / v1.z) * focalLength + 400;

        int y1 =
            300 - (v1.y / v1.z) * focalLength;

        int x2 =
            (v2.x / v2.z) * focalLength + 400;

        int y2 =
            300 - (v2.y / v2.z) * focalLength;

        // =========================
        // vertex normals
        // =========================

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

        // =========================
        // rasterize
        // =========================

        fb.drawTriangle(
            x0, y0, v0.z,
            x1, y1, v1.z,
            x2, y2, v2.z,
            i0, i1, i2
        );
    }

    fb.savePNG("output.png");

    return 0;
}