#include "SandEnginePCH.h"
#include "Camera.h"
#include "SandBase/Math/SMath.h"

Camera::Camera()
{
	m_theta = 90.0f;
	m_phi   = 90.0f;
	m_Front = SVector3f(0.0f, 0.0f, 1.0f);
	m_Up    = SVector3f(0.0f, 1.0f, 0.0f);
	m_Right = SVector3f(1.0f, 0.0f, 0.0f);

	m_near   = 0.01f;
	m_far    = 1000.0f;
	m_aspect = 1.0f;

	m_View    = SMatrix4f::Identity();
	m_Project = SMatrix4f::Identity();
}

SVector3f Camera::GetPostion() const
{
	return m_position;
}

void Camera::InitCamera(float _near, float _far, float fovy, float aspect, SVector3f & cam_pos, SVector3f & target)
{
	m_near        = _near;
	m_far         = _far;
	m_aspect      = aspect;
	m_tanHalfFovy = SMath::TanDeg(fovy / 2.0f);
	m_tanHalfFovX = m_tanHalfFovy * m_aspect;
	m_position    = cam_pos;

	m_theta = 90.0f;
	m_phi   = 90.0f;

	m_Front = SVector3f::Normalize(target - m_position);
	UpdateView();

	m_Project = SMatrix4f(	1.0f / (m_aspect * m_tanHalfFovy), 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f / m_tanHalfFovy, 0.0f, 0.0f,
							0.0f, 0.0f, m_far / (m_far - m_near), 1.0f,
							0.0f, 0.0f, -m_near * m_far / (m_far - m_near), 0.0f);
}

const SMatrix4f & Camera::GetViewMatrix()
{
	return m_View;
}

const SMatrix4f & Camera::GetProjectMatrix()
{
	
	return m_Project;
}

float Camera::GetTanHalfFovY()
{
	return m_tanHalfFovy;
}

float Camera::GetTanHalfFovX()
{
	return m_tanHalfFovX;
}

float Camera::GetAspectRatio()
{
	return m_aspect;
}

float Camera::GetNearPlane()
{
	return m_near;
}

float Camera::GetFarPlane()
{
	return m_far;
}

const SMatrix4f& Camera::GetCameraToWorldMatrix()
{
	return m_CameraToWorld;
}

void Camera::Rotate(float x, float y)
{
	m_phi   += x * -0.2f;
	m_theta += y * 0.2f;
	
	m_phi   = m_phi > 360.f ? m_phi - 360.f : (m_phi < 0.0f ? m_phi + 360.0f : m_phi);
	m_phi   = SMath::clamp(m_phi, 0.f, 360.f);
	m_theta = SMath::clamp(m_theta, 1.f, 179.f );

	m_Front = SVector3f::Normalize(SVector3f(SMath::SinDeg(m_theta) * SMath::CosDeg(m_phi), SMath::CosDeg(m_theta), SMath::SinDeg(m_theta) * SMath::SinDeg(m_phi)));

	UpdateView();
}

void Camera::MoveFront(float delta)
{
	m_position += m_Front * delta;
	UpdateView();
}

void Camera::MoveRight(float delta)
{
	m_position += m_Right * delta;
	UpdateView();
}

void Camera::SetLastMousePressPos(const SVector2f& pos)
{
	m_LastMousePressPos = pos;
}

SVector2f Camera::GetLastMousePressPos()
{
	return m_LastMousePressPos;
}

void Camera::UpdateView()
{
	m_Up    = SVector3f(0.0f, 1.0f, 0.0f);
	m_Right = SVector3f::Normalize(SVector3f::cross(m_Up, m_Front));
	m_Up    = SVector3f::Normalize(SVector3f::cross(m_Front, m_Right));

	m_View = SMatrix4f( m_Right.x, m_Up.x, m_Front.x, 0.0f,
						m_Right.y, m_Up.y, m_Front.y, 0.0f,
						m_Right.z, m_Up.z, m_Front.z, 0.0f,
						-dot(m_position, m_Right), -dot(m_position, m_Up), -dot(m_position, m_Front), 1.0f);

	m_CameraToWorld = m_View.Inverse();
}