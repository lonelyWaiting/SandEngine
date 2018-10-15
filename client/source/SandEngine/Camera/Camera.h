#pragma once

#include "SandEngine/SApiDef.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SMatrix4f.h"

class Camera
{
public:
	SAND_API SVector3f GetPostion() const;

	SAND_API void InitCamera(float _near, float _far, float fov, float aspect, SVector3f& cam_pos, SVector3f& target);
	
	SAND_API const SMatrix4f& GetViewMatrix();
	SAND_API const SMatrix4f& GetProjectMatrix();
	SAND_API const SMatrix4f& GetCameraToWorldMatrix();
	SAND_API float GetTanHalfFovY();
	SAND_API float GetTanHalfFovX();
	SAND_API float GetAspectRatio();
	SAND_API float GetFarPlane();
	SAND_API float GetNearPlane();

private:
	SVector3f m_position;
	SVector3f m_target;
	float     m_near;
	float     m_far;
	float     m_tanHalfFovy;
	float	  m_tanHalfFovX;
	float     m_aspect;

	SMatrix4f m_View;
	SMatrix4f m_Project;
	SMatrix4f m_CameraToWorld;
};