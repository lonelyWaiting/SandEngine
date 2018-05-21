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

BlendState & RenderState::GetBlendState()
{
	return m_BlendState;
}

RasterizerState& RenderState::GetRasterizerState()
{
	return m_RasterizerState;
}

RasterizerState::~RasterizerState()
{
	SAFE_RELEASE( pRasterizerState );
}