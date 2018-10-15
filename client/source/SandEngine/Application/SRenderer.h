#pragma once

class SVector2f;
class SVector4f;

#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandEngine/Camera/Camera.h"

class SRenderer
{
public:
	static SRenderer& Get();

	bool Init( HWND hwnd );
	void Shutdown();
	void Resize( const SVector2f& size );
	void ClearColor( const SVector4f& color );
	void Present( suInt32 syncInterval = 0 , suInt32 presentFlag = 0 );

	Camera& GetMainCamera();

	ID3D11RasterizerState* GetRasterizerState() { return rasterizerState; }
	ID3D11DepthStencilState* GetDepthStencilState() { return depthStencilState; }

private:
	SRenderer() {}

private:
	SRenderTexturePtr m_RT;
	Camera m_MainCamera;

	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
};