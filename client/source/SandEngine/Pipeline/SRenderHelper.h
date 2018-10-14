#pragma once

#include "SandEngine/SApiDef.h"

class SShader;
class STexture2D;
class SRenderTexture;

enum eShaderStage
{
	eST_Vertex,
	eST_Pixel,
	eST_Geometry,
	eST_Hull,
	eST_Domain,
	eST_Compute,
	eST_Max
};

class SAND_API SRenderHelper
{
public:
	static ID3D11Device*        g_Device;
	static ID3D11DeviceContext* g_ImmediateContext;
	static IDXGISwapChain*		g_SwapChain;

	static void SetRenderState();
	static void ResetStream();
	static void AddMeshStream( class SMeshBuffer* mesh , suInt32 mask );
	static bool CreateDeviceAndContext( HWND hwnd );
	static void ReportDetailDebug();
	static void BindTexture(eShaderStage stage, int slot, STexture2D* tex);
	static void SetRenderTarget(SRenderTexture* tex, bool useDepth);
	static void RenderFullScreen(const SShader& shader);
	static void Render(int indexCount, int startIndex, int startVertex, D3D11_PRIMITIVE_TOPOLOGY topology, const SShader& shader);
};