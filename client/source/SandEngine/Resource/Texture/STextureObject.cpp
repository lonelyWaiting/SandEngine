#include "SandEnginePCH.h"
#include "STextureObject.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

STextureObject::STextureObject( const char * filename , SResourceManager & manager )
	:SResource( filename , manager )
{
	m_Texture = nullptr;
}

void STextureObject::SetD3DTexture(ID3D11Texture2D* tex)
{
	m_Texture = tex;
}

ID3D11Texture2D* STextureObject::GetD3DTexture()
{
	return m_Texture;
}

void STextureObject::Ensureloaded()
{
	// TODO.Create D3D Texture2D
}

SRenderableTexture::SRenderableTexture(const char* name, const SRenderableConfig& cfg, SResourceManager& manager)
	:STextureObject(name, manager)
{
	m_cfg = cfg;
}

void SRenderableTexture::Ensureloaded()
{
	if (IsLoaded())	return;
	DoLoaded();
	SetResourceFlag(eRF_Loaded);
}

void SRenderableTexture::EnsureUnloaded()
{
	if (!IsLoaded())	return;
	DoUnloaded();
	RemoveResourceFlag(eRF_Loaded);
}

ID3D11RenderTargetView * SRenderableTexture::GetD3DRenderTarget()
{
	return m_renderTarget;
}

ID3D11DepthStencilView * SRenderableTexture::GetD3DDepthStencil()
{
	return m_depthStencil;
}

bool SRenderableTexture::IsLoaded()
{
	return HasFlag(eRF_Loaded);
}

void SRenderableTexture::DoLoaded()
{
	if (m_cfg.enableAsRenderTarget)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		ID3D11RenderTargetView* rtView = nullptr;
		SRenderHelper::g_Device->CreateRenderTargetView(m_Texture, &desc, &rtView);
		m_renderTarget = rtView;
		rtView->Release();
	}

	if (m_cfg.enableAsDepth)
	{
		D3D11_TEXTURE2D_DESC dsd;
		dsd.Width              = m_cfg.width;
		dsd.Height             = m_cfg.height;
		dsd.MipLevels          = 1;
		dsd.ArraySize          = 1;
		dsd.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsd.SampleDesc.Count   = 1;
		dsd.SampleDesc.Quality = 0;
		dsd.Usage              = D3D11_USAGE_DEFAULT;
		dsd.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		dsd.CPUAccessFlags     = 0;
		dsd.MiscFlags          = 0;

		ID3D11DepthStencilView* pDepthStencilView = nullptr;
		ID3D11Texture2D*		pDepthStencilBuffer = nullptr;
		if (FAILED(SRenderHelper::g_Device->CreateTexture2D(&dsd, 0, &pDepthStencilBuffer)))	return;
		if (FAILED(SRenderHelper::g_Device->CreateDepthStencilView(pDepthStencilBuffer, 0, &pDepthStencilView)))	return;
		m_Texture      = pDepthStencilBuffer;
		m_depthStencil = pDepthStencilView;
		pDepthStencilView->Release();
	}
}

void SRenderableTexture::DoUnloaded()
{
	if (m_Texture)		m_Texture      = nullptr;
	if (m_renderTarget)	m_renderTarget = nullptr;
	if (m_depthStencil)	m_depthStencil = nullptr;
}