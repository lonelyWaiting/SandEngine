#include "SandEnginePCH.h"
#include "SRenderer.h"
#include "SandBase/Vector/SArray.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"
#include "SandBase/Log/SLog.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandEngineModule.h"
#include "SandEngine/Resource/Texture/STextureManager.h"

SRenderer & SRenderer::Get()
{
	static SRenderer renderer;
	return renderer;
}

bool SRenderer::Init( HWND hwnd )
{
	if( !SRenderHelper::CreateDeviceAndContext( hwnd ) )	return false;

	ID3D11Texture2D* pBackBuffer = nullptr;
	if (FAILED(SRenderHelper::g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))	return false;
	m_RT = new SRenderTexture(pBackBuffer);
	m_RT->Create();
	pBackBuffer->Release();

	RECT clientArea;
	GetClientRect(hwnd, &clientArea);
	int clientWidth  = clientArea.right - clientArea.left;
	int clientHeight = clientArea.bottom - clientArea.top;

	ID3D11RenderTargetView* rtView = m_RT->GetRenderTargetView();
	SRenderHelper::g_ImmediateContext->OMSetRenderTargets(1, &rtView, m_RT->GetDepthStencilView());

	D3D11_VIEWPORT vp;
	vp.Width    = ( float )clientWidth;
	vp.Height   = ( float )clientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	SRenderHelper::g_ImmediateContext->RSSetViewports( 1 , &vp );

	return true;
}

void SRenderer::Resize( const SVector2f& size )
{
	if (!SRenderHelper::g_ImmediateContext || !SRenderHelper::g_SwapChain)	return;

	// d3d11 handle windows resize:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075%28v=vs.85%29.aspx#Handling_Window_Resizing
	SRenderHelper::g_ImmediateContext->OMSetRenderTargets( 0 , 0 , 0 );

	// Release all outstanding references to the swap chain's buffers
	m_RT = nullptr;

	// Preserve the existing buffer count and format
	// Automatically choose the width and height to match the client rect for HWND
	if( FAILED(SRenderHelper::g_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)) )
	{
		SLog::Error("Resize Buffer Failed");
		return;
	}

	// Get buffer and create a render-target-view
	ID3D11Texture2D* pBackBuffer = nullptr;
	if (FAILED(SRenderHelper::g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		SLog::Error("swap chain get back buffer failed!");
		return;
	}

	/*SRenderableConfig cfg;
	cfg.enableAsRenderTarget = true;
	cfg.enableAsDepth        = false;
	m_RenderTarget = SandEngine::TextureManager.CreateRenderableTexture("Final", cfg);
	m_RenderTarget->SetD3DTexture(pBackBuffer);
	m_RenderTarget->Ensureloaded();*/
	m_RT = new SRenderTexture(pBackBuffer);
	pBackBuffer->Release();

	// Perform error handling here!
	if (!m_RT->GetRenderTargetView())
	{
		SLog::Error("Resize CreateRenderTargetView Failed");
		return;
	}

	// should i recreate the depth/stencil buffer too ?
	// https://gamedev.stackexchange.com/questions/86164/idxgiswapchainresizebuffers-should-i-recreate-the-depth-stencil-buffer-too
	// depth/stencil resize is just optional

	ID3D11RenderTargetView* rtView = m_RT->GetRenderTargetView();
	SRenderHelper::g_ImmediateContext->OMSetRenderTargets(1, &rtView, m_RT->GetDepthStencilView());

	D3D11_VIEWPORT vp;
	vp.Width    = size.x;
	vp.Height   = size.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	SRenderHelper::g_ImmediateContext->RSSetViewports( 1 , &vp );
}

void SRenderer::ClearColor( const SVector4f & color )
{
	const float c[4] = { color[0] / 255.0f ,color[1] / 255.0f,color[2] / 255.0f,color[3] / 255.0f };

	SRenderHelper::g_ImmediateContext->ClearRenderTargetView(m_RT->GetRenderTargetView(), c);
	SRenderHelper::g_ImmediateContext->ClearDepthStencilView(m_RT->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );

	ID3D11RenderTargetView* rtView = m_RT->GetRenderTargetView();
	SRenderHelper::g_ImmediateContext->OMSetRenderTargets(1, &rtView, m_RT->GetDepthStencilView());
}

void SRenderer::Shutdown()
{
	SRenderHelper::ReportDetailDebug();
	SAFE_RELEASE(SRenderHelper::g_Device);
	SAFE_RELEASE(SRenderHelper::g_ImmediateContext);
	SAFE_RELEASE(SRenderHelper::g_SwapChain);
}

void SRenderer::Present( suInt32 syncInterval /*= 0*/, suInt32 presentFlag /*= 0*/ )
{
	if(SRenderHelper::g_SwapChain)	SRenderHelper::g_SwapChain->Present( syncInterval , presentFlag );
}