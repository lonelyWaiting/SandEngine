#include "SandEnginePCH.h"
#include "RenderState.h"
#include "SandEngine/Application/SRenderer.h"

BlendState::BlendState()
{
	desc.alphaToCoverageEnable          = false;
	desc.independentBlendEnable         = false;
	desc.renderTarget[0].blendEnable    = true;
	desc.renderTarget[0].srcBlend       = eBF_Src_Alpha;
	desc.renderTarget[0].destBlend      = eBF_Inv_Src_Alpha;
	desc.renderTarget[0].blendOp        = eBO_Add;
	desc.renderTarget[0].srcBlendAlpha  = eBF_One;
	desc.renderTarget[0].destBlendAlpha = eBF_Zero;
	desc.renderTarget[0].blendOpAlpha   = eBO_Add;
	pBlendState = nullptr;
}

BlendState::~BlendState()
{
	SAFE_RELEASE( pBlendState );
}

void BlendState::ComputeHash()
{
	D3D11_BLEND_DESC blendDesc;
	memcpy( &blendDesc , &desc , sizeof( desc ) );
	SRenderer::Get().GetDevice()->CreateBlendState( &blendDesc , &pBlendState );
}

BlendStateDesc & BlendState::GetDesc()
{
	return desc;
}

RasterizerState::RasterizerState()
{
	desc.fillMode              = eFM_Solid;
	desc.cullMode              = eCM_None;
	desc.frontCounterClockwise = true;
	desc.depthBias             = false;
	desc.depthBiasClamp        = 0;
	desc.slopeScaledDepthBias  = 0;
	desc.depthClipEnable       = true;
	desc.scissorEnable         = true;
	desc.multisampleEnable     = false;
	desc.antialiasedLineEnable = false;

	pRasterizerState = nullptr;
}

void RasterizerState::ComputeHash()
{
	D3D11_RASTERIZER_DESC raster_desc;
	memcpy( &raster_desc , &desc , sizeof( desc ) );
	SRenderer::Get().GetDevice()->CreateRasterizerState( &raster_desc , &pRasterizerState );
}

RasterizerStateDesc & RasterizerState::GetDesc()
{
	return desc;
}

BlendStateDesc & RenderState::GetBlendState()
{
	return m_BlendState.GetDesc();
}

RasterizerStateDesc& RenderState::GetRasterizerState()
{
	return m_RasterizerState.GetDesc();
}

DepthStencilDesc& RenderState::GetDepthStencilDesc()
{
	return m_DepthStencilState.GetDesc();
}

RasterizerState::~RasterizerState()
{
	SAFE_RELEASE( pRasterizerState );
}

#define stencil_default_read_mask 0xff
#define stencil_default_write_mask 0xff

DepthStencilState::DepthStencilState()
{
	desc.depthEnable                  = true;
	desc.depthWriteMask               = eDWM_All;
	desc.depthFunc                    = eCF_Less;
	desc.stencilEnable                = false;
	desc.stencilReadMask              = stencil_default_read_mask;
	desc.stencilWriteMask             = stencil_default_write_mask;
	desc.frontFace.stencilFunc        = eCF_Always;
	desc.frontFace.stencilDepthFailOp = eSO_Keep;
	desc.frontFace.stencilPassOp      = eSO_Keep;
	desc.frontFace.stencilFailOp      = eSO_Keep;
	desc.backFace.stencilFunc         = eCF_Always;
	desc.backFace.stencilDepthFailOp  = eSO_Keep;
	desc.backFace.stencilPassOp       = eSO_Keep;
	desc.backFace.stencilFailOp       = eSO_Keep;

	pDepthStencilState = nullptr;
}

DepthStencilDesc & DepthStencilState::GetDesc()
{
	return desc;
}