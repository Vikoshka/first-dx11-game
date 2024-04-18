#pragma once

#include <DirectXMath.h>

/*
    REMEMBER ALINGMENT OF 16 BYTES
*/
struct CB_VS_vertexshader
{
    DirectX::XMMATRIX mat;  // 64
};

struct CB_VS_origin             // 8 bytes
{
    float x = 0.0f; // 4
    float y = 0.0f; // 4
    // 8 free
};

struct CB_VS_trans              // 8 bytes
{
    float x = 0.0f; // 4
    float y = 0.0f; // 4
    // 8 free
};

struct CB_VS_scale              // 8 bytes
{
    float x = 1.0f; // 4
    float y = 1.0f; // 4
    // 8 free
};

struct CB_VS_rotation           // 4 bytes
{
    float a = 0.0f; // 4
    // 12 free
};

struct CB_VS_sprite2D           // 32 bytes
{
    CB_VS_origin origin;        // 8
    CB_VS_trans translation;    // 8
    CB_VS_scale scale;          // 8
    CB_VS_rotation rotation;    // 4
    float padding[1];           // 4
                                // 0 free
};