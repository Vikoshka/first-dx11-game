#pragma once
#include <DIrectXMath.h>

class Vertex
{
public:
    Vertex();
    Vertex(float x, float y, float z)
        : pos(x, y, z), coords(0.0f, 0.0f) {}
    Vertex(float x, float y, float z, float u, float v)
        : pos(x, y, z), coords(u, v) {}

private:
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 coords;
};

