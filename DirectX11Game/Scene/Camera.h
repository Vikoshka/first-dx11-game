#pragma once

#include <../Common/CommonMath.h>
#include <memory>
using namespace DirectX;

class Camera
{
public:
    Camera();
    void SetProjectionValues(float width, float height);

    const XMFLOAT2& GetPosition() const;
    const float& GetRotation() const;
    const float& GetRotationDeg() const;

    void SetPosition(const XMFLOAT2& pos);
    void SetRotation(const float& ang);
    void SetRotationDeg(const float& ang);
    void AdjustPosition(const XMFLOAT2& vec);
    void AdjustRotation(const float& ang);
    void AdjustRotationDeg(const float& ang);

private:
    void UpdateViewMatrix();

    MATRIX2D m_viewMatrix;
    XMFLOAT2 m_projection = { 0.0f, 0.0f };
    XMFLOAT2 m_position = { 0.0f, 0.0f };
    float m_rotation = 0.0f;

};