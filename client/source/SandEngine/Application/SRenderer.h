#pragma once

class SVector2f;
class SVector4f;

#include "SandEngine/Resource/Texture/STextureObject.h"

class SRenderer
{
public:
	static SRenderer& Get();

	bool Init( HWND hwnd );
	void Shutdown();
	void Resize( const SVector2f& size );
	void ClearColor( const SVector4f& color );
	void Present( suInt32 syncInterval = 0 , suInt32 presentFlag = 0 );

private:
	SRenderer() {}

private:
	SRenderableTexturePtr m_RenderTarget;
	SRenderableTexturePtr m_DepthStencil;
};