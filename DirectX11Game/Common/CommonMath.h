#pragma once

#include <DirectXMath.h>
#include <cmath>

#define MATRIX2D matrix2d

struct matrix2d
{
    float _11, _12;
    float _21, _22;
};

using namespace DirectX;

namespace Math2D
{
    inline MATRIX2D Matrix2dMultiply(MATRIX2D mat1, MATRIX2D mat2)
    {
        MATRIX2D result;
        result._11 = mat1._11 * mat2._11 + mat1._12 * mat2._21;
        result._12 = mat1._11 * mat2._12 + mat1._12 * mat2._22;
        result._21 = mat1._21 * mat2._11 + mat1._22 * mat2._21;
        result._22 = mat1._21 * mat2._12 + mat1._22 * mat2._22;
        return result;
    };

    inline MATRIX2D Matrix2dTrans(MATRIX2D mat)
    {
        MATRIX2D result;
        result = mat;
        result._21 = mat._12;
        result._12 = mat._21;
        return result;
    };

    inline MATRIX2D Matrix2dIdentity()
    {
        MATRIX2D result;
        result._11 = 1.0f;
        result._22 = 1.0f;
        return result;
    };

    MATRIX2D Matrix2dScale(XMFLOAT2 vec2)
    {
        MATRIX2D result;
        result._11 = vec2.x;
        result._22 = vec2.y;
        return result;
    };

    MATRIX2D Matrix2dRotation(float ang)
    {
        MATRIX2D result;
        result._11 = cos(ang);
        result._12 = sin(ang);
        result._21 = sin(ang) * -1;
        result._22 = cos(ang);
        return result;
    };

    XMFLOAT2 Vector2dMulMatrix2d(XMFLOAT2 vec2, MATRIX2D mat)
    {
        XMFLOAT2 result;
        result.x = vec2.x * mat._11 + vec2.x * mat._21;
        result.y = vec2.y * mat._21 + vec2.y * mat._22;
        return result;
    };
};
