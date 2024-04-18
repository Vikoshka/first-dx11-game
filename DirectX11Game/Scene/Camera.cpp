#include "Camera.h"

Camera::Camera()
{
    
}

void Camera::SetProjectionValues(float width, float height)
{
    
}

const XMFLOAT2& Camera::GetPosition() const
{
    // // O: insert return statement here
    return m_position;
}

const float& Camera::GetRotation() const
{
    // // O: insert return statement here
    return m_rotation;
}

const float& Camera::GetRotationDeg() const
{
    // // O: insert return statement here
    return m_rotation / XM_PIDIV2 * 180.0f;
}

void Camera::SetPosition(const XMFLOAT2& pos)
{
    m_position = pos;
}

void Camera::SetRotation(const float& ang)
{
    m_rotation = ang;
}

void Camera::SetRotationDeg(const float& ang)
{
    m_rotation = ang * XM_PIDIV2 / 180.0f;
}

void Camera::AdjustPosition(const XMFLOAT2& vec)
{
    m_position.x += vec.x;
    m_position.y += vec.y;
}

void Camera::AdjustRotation(const float& ang)
{
    m_rotation += ang;
}

void Camera::AdjustRotationDeg(const float& ang)
{
    m_rotation += ang * XM_PIDIV2 / 180.0f;
}

void Camera::UpdateViewMatrix()
{
    m_viewMatrix = Math2D::Matrix2dIdentity();

    
}
