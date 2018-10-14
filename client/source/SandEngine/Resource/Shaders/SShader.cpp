#include "SandEnginePCH.h"
#include "SShader.h"
#include "SandBase/IO/SFileStream.h"
#include "SandBase/Log/SLog.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

bool SShader::Load(const char* path, const char* vs_main, const char* ps_main, const char* gs_main /*= nullptr*/, const char* hs_main /*= nullptr*/, const char* ds_main /*= nullptr*/, const char* cs_main /*= nullptr*/)
{
	char* data = nullptr;
	int   size = 0;

	SFileInStream fi;
	if (fi.OpenFile(path))
	{
		size = fi.GetFileSize();
		data = new char[size];
		fi.Read(data, size);
		fi.Close();
	}

	if (!data)	return false;

	ID3DBlob* errorBlob = nullptr;

	if (vs_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								vs_main,
								"vs_5_0",
								0,
								0,
								&mVertexBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("vertex shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreateVertexShader(mVertexBlob->GetBufferPointer(), mVertexBlob->GetBufferSize(), nullptr, &mVertexShader);
	}

	if (ps_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								ps_main,
								"ps_5_0",
								0,
								0,
								&mPixelBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("pixel shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreatePixelShader(mPixelBlob->GetBufferPointer(), mPixelBlob->GetBufferSize(), nullptr, &mPixelShader);
	}

	if (gs_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								gs_main,
								"gs_5_0",
								0,
								0,
								&mGeometryBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("geometry shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreateGeometryShader(mGeometryBlob->GetBufferPointer(), mGeometryBlob->GetBufferSize(), nullptr, &mGeometryShader);
	}

	if (hs_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								hs_main,
								"hs_5_0",
								0,
								0,
								&mHullBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("hull shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreateHullShader(mHullBlob->GetBufferPointer(), mHullBlob->GetBufferSize(), nullptr, &mHullShader);
	}

	if (ds_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								ds_main,
								"ds_5_0",
								0,
								0,
								&mDomainBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("domain shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreateDomainShader(mDomainBlob->GetBufferPointer(), mDomainBlob->GetBufferSize(), nullptr, &mDomainShader);
	}

	if (cs_main != nullptr)
	{
		HRESULT hr = D3DCompile(data,
								size,
								nullptr,
								nullptr,
								D3D_COMPILE_STANDARD_FILE_INCLUDE,
								cs_main,
								"cs_5_0",
								0,
								0,
								&mComputeBlob,
								&errorBlob);

		if (FAILED(hr) && errorBlob)
		{
			SLog::Error("compute shader compile failed: %s", (char*)errorBlob->GetBufferPointer());
			return false;
		}

		SRenderHelper::g_Device->CreateComputeShader(mComputeBlob->GetBufferPointer(), mComputeBlob->GetBufferSize(), nullptr, &mComputeShader);
	}

	SAFE_DELETE_ARRAY(data);

	return true;
}

#include <map>
#include "SandBase/String/SString.h"

static std::map<std::string, SShader> g_shaderManager;

const SShader& EnsureShaderLoaded(	const char* path, 
									const char* vs_main, 
									const char* ps_main, 
									const char* gs_main /*= nullptr*/,
									const char* hs_main /*= nullptr*/, 
									const char* ds_main /*= nullptr*/, 
									const char* cs_main /*= nullptr*/)
{
	SShader& shader = g_shaderManager[path];
	shader.Load(path, vs_main, ps_main, gs_main, hs_main, ds_main, cs_main);
	return shader;
}

void SShader::InitShader()
{
	EnsureShaderLoaded("../data/shaders/fullscreenVS.hlsl", "vs_main", nullptr, nullptr, nullptr, nullptr, nullptr);
}

const SShader& SShader::FindShader(const char* path)
{
	auto iter = g_shaderManager.find(path);
	if (iter != g_shaderManager.end())	return g_shaderManager[path];

	static SShader temp;
	return temp;
}