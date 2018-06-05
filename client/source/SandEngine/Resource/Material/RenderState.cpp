#include "SandEnginePCH.h"
#include "RenderState.h"
#include "SandEngine/Application/SRenderer.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

unsigned int ComputeHashInternal( const void* ptr , int size )
{
	unsigned int hash = 0;
	int sizeInt = size >> 2;
	unsigned int* pIntData = ( unsigned int* )ptr;
	for( int i = 0; i < sizeInt; i++ , pIntData++ )
	{
		hash += ( unsigned int )*pIntData * ( i + 11 );
	}

	int sizeChar = size - ( sizeInt << 2 );
	unsigned char* pCharData = ( unsigned char* )pIntData;
	for( int i = 0; i < sizeChar; i++ , pCharData++ )
	{
		hash += ( unsigned char )*pCharData * ( i + 13 );
	}

	return hash;
}

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
	hash = ComputeHashInternal( &desc , sizeof( desc ) );

	ID3D11BlendState* pState = nullptr;
	D3D11_BLEND_DESC blendDesc;
	memcpy( &blendDesc , &desc , sizeof( blendDesc ) );
	SRenderHelper::g_Device->CreateBlendState( &blendDesc , &pState );
	pBlendState = pState;
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
	hash = ComputeHashInternal( &desc , sizeof( desc ) );

	ID3D11RasterizerState* pState = nullptr;
	D3D11_RASTERIZER_DESC raster_desc;
	memcpy( &raster_desc , &desc , sizeof( desc ) );
	SRenderHelper::g_Device->CreateRasterizerState( &raster_desc , &pState );
	pRasterizerState = pState;
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

void DepthStencilState::ComputeHash()
{
	hash = ComputeHashInternal( &desc , sizeof( desc ) );

	ID3D11DepthStencilState* pState = nullptr;
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	memcpy( &dsDesc , &desc , sizeof( dsDesc ) );
	SRenderHelper::g_Device->CreateDepthStencilState( &dsDesc , &pState );
	pDepthStencilState = pState;
}