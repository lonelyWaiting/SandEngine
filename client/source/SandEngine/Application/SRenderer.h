#pragma once

class SVector2f;
class SVector4f;

class SRenderer
{
public:
	static SRenderer& Get();

	bool Init( HWND hwnd );
	void Shutdown();
	void Resize( const SVector2f& size );
	void ClearColor( const SVector4f& color );
	void Present( suInt32 syncInterval = 0 , suInt32 presentFlag = 0 );

public:
	ID3D11Device * GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	SRenderer() {}

private:
	ID3D11Device *          m_pDevice              = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext    = nullptr;
	IDXGISwapChain*         m_pSwapChain           = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView    = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView    = nullptr;
};