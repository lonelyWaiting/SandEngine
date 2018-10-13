#pragma once

#include "SandEngine/SApiDef.h"

enum  eShaderStage;
class STextureObject;
class SShader;
class SVector4f;
class SRenderableTexture;

class SRenderHelper
{
public:
	static ID3D11Device*        g_Device;
	static ID3D11DeviceContext* g_ImmediateContext;
	static IDXGISwapChain*		g_SwapChain;

	static void SetRenderState();
	static void ResetStream();
	static void AddMeshStream( class SMeshBuffer* mesh , suInt32 mask );
	static void SetTexture( eShaderStage stage , int slot , STextureObject* tex );
	static bool CreateDeviceAndContext( HWND hwnd );
	static void CreateShader( SShader& shader );
	static void ReportDetailDebug();
};