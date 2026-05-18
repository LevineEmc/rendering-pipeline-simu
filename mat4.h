#ifndef MAT4_H
#define MAT4_H

#pragma once

#include <cmath>

struct Vec4
{
    float x, y, z, w;

    Vec4()
        : x(0), y(0), z(0), w(1) {}

    Vec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}
};

struct Mat4
{
    float m[4][4];

    // 单位矩阵
    static Mat4 identity()
    {
        Mat4 r = {};

        r.m[0][0] = 1;
        r.m[1][1] = 1;
        r.m[2][2] = 1;
        r.m[3][3] = 1;

        return r;
    }

    // 矩阵乘矩阵
    Mat4 operator*(const Mat4& o) const
    {
        Mat4 r = {};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    r.m[i][j] +=
                        m[i][k] * o.m[k][j];
                }
            }
        }

        return r;
    }

    // 矩阵乘Vec4
    Vec4 operator*(const Vec4& v) const
    {
        Vec4 r;

        r.x =
            m[0][0]*v.x +
            m[0][1]*v.y +
            m[0][2]*v.z +
            m[0][3]*v.w;

        r.y =
            m[1][0]*v.x +
            m[1][1]*v.y +
            m[1][2]*v.z +
            m[1][3]*v.w;

        r.z =
            m[2][0]*v.x +
            m[2][1]*v.y +
            m[2][2]*v.z +
            m[2][3]*v.w;

        r.w =
            m[3][0]*v.x +
            m[3][1]*v.y +
            m[3][2]*v.z +
            m[3][3]*v.w;

        return r;
    }

    // 平移矩阵
    static Mat4 translation(
        float x,
        float y,
        float z)
    {
        Mat4 r = identity();

        r.m[0][3] = x;
        r.m[1][3] = y;
        r.m[2][3] = z;

        return r;
    }

    // 绕轴旋转
    static Mat4 rotationX(float angle)
    {
        Mat4 r = identity();

        float c = cos(angle);
        float s = sin(angle);

        r.m[1][1] = c;
        r.m[1][2] = -s;

        r.m[2][1] = s;
        r.m[2][2] = c;

        return r;
    }
    static Mat4 rotationY(float angle)
    {
        Mat4 r = identity();

        float c = cos(angle);
        float s = sin(angle);

        r.m[0][0] = c;
        r.m[0][2] = s;

        r.m[2][0] = -s;
        r.m[2][2] = c;

        return r;
    }
    static Mat4 rotationZ(float angle)
    {
        Mat4 r = identity();

        float c = cos(angle);
        float s = sin(angle);

        r.m[0][0] = c;
        r.m[0][1] = -s;

        r.m[1][0] = s;
        r.m[1][1] = c;

        return r;
    }
    static Mat4 rotation(
    float pitch,
    float yaw,
    float roll)
    {
        Mat4 rx = rotationX(pitch);
        Mat4 ry = rotationY(yaw);
        Mat4 rz = rotationZ(roll);

        return rz * ry * rx;
    }

    static Mat4 perspective(
    float fov,
    float aspect,
    float near,
    float far)
    {
        Mat4 r = {};

        float tanHalfFov =
            tan(fov / 2.0f);

        r.m[0][0] =
            1.0f / (aspect * tanHalfFov);

        r.m[1][1] =
            1.0f / tanHalfFov;

        r.m[2][2] =
            -(far + near) / (far - near);

        r.m[2][3] =
            -(2.0f * far * near) / (far - near);

        r.m[3][2] = -1.0f;

        return r;
    }
};

#endif