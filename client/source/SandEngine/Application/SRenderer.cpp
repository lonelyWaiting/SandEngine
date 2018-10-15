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

	m_MainCamera.InitCamera(0.1f, 1000.0f, 75.0f, clientWidth / (float)clientHeight, SVector3f(0.0f, 0.0f, -30.0f), SVector3f(0.0f, 0.0f, 1.0f));

	// ------------------------------------------Depth/Stencil State------------------------------------------
	//bool MSAAEnabled = s3Renderer::get().getMSAAEnabled();

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilStateDesc.DepthEnable = false;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilStateDesc.StencilEnable = FALSE;

	HRESULT hr = SRenderHelper::g_Device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
	if (FAILED(hr))
	{
		SLog::Error("Failed to create DepthStencil State\n");
		return false;
	}

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	hr = SRenderHelper::g_Device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if (FAILED(hr))
	{
		SLog::Error("Failed to create Rasterrizer State\n");
		return false;
	}

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

Camera& SRenderer::GetMainCamera()
{
	return m_MainCamera;
}