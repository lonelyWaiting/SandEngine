#pragma once

#include "SandEngine/SApidef.h"

class SAND_API SMaterial
{
public:
	SMaterial(const char* filename);

public:
	void SetFloat(const char* name, float v);
	void SetFloat2(const char* name, class SVector2f v);
	void SetFloat3(const char* name, class SVector3f v);
	void SetFloat4(const char* name, class SVector4f v);
	void SetInt(const char* name, int v);
	void SetBool(const char* name, bool v);
	void SetMatrix(const char* name, class SMatrix4f matrix);
	void SetTexture(const char* name, class STexture2D* tex);
	void SetTextureOffset(const char* name, SVector2f offset);
	void SetTextureScale(const char* name, SVector2f scale);
	void SetPass(int pass);
	void Apply();

	void EnableKeyword(const char* keyword);
	void DisableKeyword(const char* keyword);

private:
	class SMaterialImp* MatImp;
};