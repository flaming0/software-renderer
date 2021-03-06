/*
 * m33.cpp
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#include "stdafx.h"

#include "m33.h"

namespace math
{

M33::M33()
{
    reset();
}

M33::M33(const float (&src)[3][3])
{
    set(src);
}

M33::M33(float a00, float a01, float a02,
         float a10, float a11, float a12,
         float a20, float a21, float a22)
{
    set(a00, a01, a02,
        a10, a11, a12,
        a20, a21, a22);
}

void M33::set(const float (&src)[3][3])
{
    memcpy(x, src, 3 * 3 * sizeof(float));
}

void M33::set(float a00, float a01, float a02,
              float a10, float a11, float a12,
              float a20, float a21, float a22)
{
    x[0][0] = a00;
    x[0][1] = a01;
    x[0][2] = a02;
    x[1][0] = a10;
    x[1][1] = a11;
    x[1][2] = a12;
    x[2][0] = a20;
    x[2][1] = a21;
    x[2][2] = a22;
}

void M33::reset()
{
    memset(&x[0], 0, sizeof(float) * 3 * 3);
    x[0][0] = x[1][1] = x[2][2] = 1.0;
}

M33 &M33::operator+= (const M33 &a)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            x[i][j] += a.x[i][j];

    return *this;
}

M33 &M33::operator-= (const M33 &a)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            x[i][j] -= a.x[i][j];

    return *this;
}

M33 &M33::operator*= (const M33 &a)
{
    M33 temp = *this;

    x[0][0] = temp.x[0][0] * a.x[0][0] + temp.x[0][1] * a.x[1][0] + temp.x[0][2] * a.x[2][0];
    x[0][1] = temp.x[0][0] * a.x[0][1] + temp.x[0][1] * a.x[1][1] + temp.x[0][2] * a.x[2][1];
    x[0][2] = temp.x[0][0] * a.x[0][2] + temp.x[0][1] * a.x[1][2] + temp.x[0][2] * a.x[2][2];
    x[1][0] = temp.x[1][0] * a.x[0][0] + temp.x[1][1] * a.x[1][0] + temp.x[1][2] * a.x[2][0];
    x[1][1] = temp.x[1][0] * a.x[0][1] + temp.x[1][1] * a.x[1][1] + temp.x[1][2] * a.x[2][1];
    x[1][2] = temp.x[1][0] * a.x[0][2] + temp.x[1][1] * a.x[1][2] + temp.x[1][2] * a.x[2][2];
    x[2][0] = temp.x[2][0] * a.x[0][0] + temp.x[2][1] * a.x[1][0] + temp.x[2][2] * a.x[2][0];
    x[2][1] = temp.x[2][0] * a.x[0][1] + temp.x[2][1] * a.x[1][1] + temp.x[2][2] * a.x[2][1];
    x[2][2] = temp.x[2][0] * a.x[0][2] + temp.x[2][1] * a.x[1][2] + temp.x[2][2] * a.x[2][2];

    return *this;
}

M33 &M33::operator*= (float s)
{
    for (auto &row : x)
        for (auto &el : row)
            el *= s;

    return *this;
}

M33 &M33::operator/= (float s)
{
    assert(!DCMP(s, 0.0));

    for (auto &row : x)
        for (auto &el : row)
            el /= s;

    return *this;
}

bool M33::operator== (const M33 &a) const
{
    return DCMP(x[0][0], a.x[0][0]) && DCMP(x[0][1], a.x[0][1]) && DCMP(x[0][2], a.x[0][2]) &&
           DCMP(x[1][0], a.x[1][0]) && DCMP(x[1][1], a.x[1][1]) && DCMP(x[1][2], a.x[1][2]) &&
           DCMP(x[2][0], a.x[2][0]) && DCMP(x[2][1], a.x[2][1]) && DCMP(x[2][2], a.x[2][2]);
}

M33 &M33::invert()
{
    float det = determinant();
    assert(!DCMP(det, 0.0));

    M33 res;
    res.x[0][0] = (x[1][1] * x[2][2] - x[1][2] * x[2][1]) / det;
    res.x[0][1] = (x[0][2] * x[2][1] - x[0][1] * x[2][2]) / det;
    res.x[0][2] = (x[0][1] * x[1][2] - x[0][2] * x[1][1]) / det;
    res.x[1][0] = (x[1][2] * x[2][0] - x[1][0] * x[2][2]) / det;
    res.x[1][1] = (x[0][0] * x[2][2] - x[0][2] * x[2][0]) / det;
    res.x[1][2] = (x[0][2] * x[1][0] - x[0][0] * x[1][2]) / det;
    res.x[2][0] = (x[1][0] * x[2][1] - x[1][1] * x[2][0]) / det;
    res.x[2][1] = (x[0][1] * x[2][0] - x[0][0] * x[2][1]) / det;
    res.x[2][2] = (x[0][0] * x[1][1] - x[0][1] * x[1][0]) / det;

    return *this = res;
}

M33 &M33::transpose()
{
    M33 res;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            res.x[i][j] = x[j][i];

    return *this = res;
}

float M33::determinant() const
{
    return (x[0][0] * (x[1][1] * x[2][2] - x[1][2] * x[2][1]) -
            x[0][1] * (x[1][0] * x[2][2] - x[1][2] * x[2][0]) +
            x[0][2] * (x[1][0] * x[2][1] - x[1][1] * x[2][0]));
}

M33 M33::getScaleMatrix(const vec3 &vect)
{
    M33 A;

    A.x[0][0] = vect.x;
    A.x[1][1] = vect.y;
    A.x[2][2] = vect.z;

    return A;
}

M33 M33::getRotateXMatrix(float angle, bool rads)
{
    M33 A;

    float cosinus = rads ? cos(angle) : cos(DegToRad(angle));
    float sinus = rads ? sin(angle) : sin(DegToRad(angle));

    A.x[1][1] = cosinus;
    A.x[1][2] = sinus;
    A.x[2][1] = -sinus;
    A.x[2][2] = cosinus;

    return A;
}

M33 M33::getRotateYMatrix(float angle, bool rads)
{
    M33 A;

    float cosinus = rads ? cos(angle) : cos(DegToRad(angle));
    float sinus = rads ? sin(angle) : sin(DegToRad(angle));

    A.x[0][0] = cosinus;
    A.x[0][2] = -sinus;
    A.x[2][0] = sinus;
    A.x[2][2] = cosinus;

    return A;
}

M33 M33::getRotateZMatrix(float angle, bool rads)
{
    M33 A;

    float cosinus = rads ? cos(angle) : cos(DegToRad(angle));
    float sinus = rads ? sin(angle) : sin(DegToRad(angle));

    A.x[0][0] = cosinus;
    A.x[0][1] = sinus;
    A.x[1][0] = -sinus;
    A.x[1][1] = cosinus;

    return A;
}

M33 M33::getRotateYawPitchRollMatrix(float yaw, float pitch, float roll, bool rads)
{
    // yxz or zyx!
    return getRotateYMatrix(yaw, rads) *
           getRotateXMatrix(pitch, rads) *
           getRotateZMatrix(roll, rads);
}

M33 operator+ (const M33 &a, const M33 &b)
{
    M33 res;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            res.x[i][j] = a.x[i][j] + b.x[i][j];

    return res;
}

M33 operator- (const M33 &a, const M33 &b)
{
    M33 res;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            res.x[i][j] = a.x[i][j] - b.x[i][j];

    return res;
}

M33 operator* (const M33 &a, const M33 &b)
{
    M33 c = a;

    c.x[0][0] = a.x[0][0] * b.x[0][0] + a.x[0][1] * b.x[1][0] + a.x[0][2] * b.x[2][0];
    c.x[0][1] = a.x[0][0] * b.x[0][1] + a.x[0][1] * b.x[1][1] + a.x[0][2] * b.x[2][1];
    c.x[0][2] = a.x[0][0] * b.x[0][2] + a.x[0][1] * b.x[1][2] + a.x[0][2] * b.x[2][2];
    c.x[1][0] = a.x[1][0] * b.x[0][0] + a.x[1][1] * b.x[1][0] + a.x[1][2] * b.x[2][0];
    c.x[1][1] = a.x[1][0] * b.x[0][1] + a.x[1][1] * b.x[1][1] + a.x[1][2] * b.x[2][1];
    c.x[1][2] = a.x[1][0] * b.x[0][2] + a.x[1][1] * b.x[1][2] + a.x[1][2] * b.x[2][2];
    c.x[2][0] = a.x[2][0] * b.x[0][0] + a.x[2][1] * b.x[1][0] + a.x[2][2] * b.x[2][0];
    c.x[2][1] = a.x[2][0] * b.x[0][1] + a.x[2][1] * b.x[1][1] + a.x[2][2] * b.x[2][1];
    c.x[2][2] = a.x[2][0] * b.x[0][2] + a.x[2][1] * b.x[1][2] + a.x[2][2] * b.x[2][2];

    return c;
}

M33 operator* (const M33 &a, float s)
{
    M33 res = a;

    return res *= s;
}

M33 operator* (float s, const M33 &b)
{
    M33 res = b;

    return res *= s;
}

vec3 operator* (const vec3 &v, const M33 &a)
{
    return vec3(a.x[0][0] * v.x + a.x[1][0] * v.y + a.x[2][0] * v.z,
                a.x[0][1] * v.x + a.x[1][1] * v.y + a.x[2][1] * v.z,
                a.x[0][2] * v.x + a.x[1][2] * v.y + a.x[2][2] * v.z);
}

}
