#pragma once

#include "RenderState.h"
#include "SandBase/String/SString.h"
#include "Resource/SConstantBuffer.h"
#include "Resource/Shaders/SShader.h"

class SMaterial
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
	int ValidateParam();

private:
	const SShader* shader = nullptr;
	int curPass = -1;
	SArray<SString> keywordList;

	union paramValue
	{
		float	  vf;
		SVector2f vf2;
		SVector3f vf3;
		SVector4f vf4;
		int		  vi;
		SMatrix4f vm;
		bool	  vb;
	};

	struct sTextureInfo
	{
		STexture2DPtr texture;
		SVector2f scale;
		SVector2f offset;

		sTextureInfo() { texture = nullptr; scale = SVector2f(1.0f, 1.0f); offset = SVector2f(0.0f, 0.0f); }
	};

	std::map<shaderParameter, paramValue>	  paramList;
	std::map<const char*, sTextureInfo>       textureList;
	std::map<const char*, SConstantBufferPtr> cbList;
};