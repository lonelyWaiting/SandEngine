#include "SandEnginePCH.h"
#include "Camera.h"
#include "SandBase/Math/SMath.h"

SVector3f Camera::GetPostion() const
{
	return m_position;
}

void Camera::InitCamera(float _near, float _far, float fovy, float aspect, SVector3f & cam_pos, SVector3f & target)
{
	m_near = _near;
	m_far = _far;
	m_aspect = aspect;
	m_tanHalfFovy = SMath::TanDeg(fovy / 2.0f);
	m_tanHalfFovX = m_tanHalfFovy * m_aspect;
	m_position = cam_pos;
	m_target = target;

	SVector3f front = SVector3f::Normalize(m_target - m_position);
	SVector3f up = front.y < 0.999f ? SVector3f(0.0f, 1.0f, 0.0f) : SVector3f(0.0f, 0.0f, -1.0f);
	SVector3f right = SVector3f::Normalize(SVector3f::cross(up, front));
	up = SVector3f::Normalize(SVector3f::cross(front, right));

	m_View = SMatrix4f(right.x,			right.y,		right.z,		0.0f,
					   up.x,			up.y,			up.z,			0.0f,
					   front.x,			front.y,		front.z,		0.0f,
					   -m_position.x,	-m_position.y, -m_position.z,	1.0f);

	m_CameraToWorld = m_View.Inverse();

	m_Project = SMatrix4f(	1.0f / (m_aspect * m_tanHalfFovy), 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f / (m_aspect * m_tanHalfFovy), 0.0f, 0.0f,
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