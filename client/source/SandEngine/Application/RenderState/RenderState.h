#pragma once

#include "SandEngine/SandEnginePCH.h"
#include "SandBase/Object/SRefCounter.h"
#include "SandBase/Object/SSmartPointer.h"

enum eBlendFactor
{
	eBF_Zero             = 0,
	eBF_One              = 1,
	eBF_Src_Color        = 2,
	eBF_Inv_Src_Color    = 3,
	eBF_Src_Alpha		 = 4,
	eBF_Inv_Src_Alpha	 = 5,
	eBF_Dest_Alpha       = 6,
	eBF_Inv_Dest_Alpha   = 7,
	eBF_Dest_Color       = 8,
	eBF_Inv_Dest_Color   = 9,
	eBF_Src_Alpha_Sat    = 10,
	eBF_Blend_Factor     = 11,
	eBF_Inv_Blend_Factor = 14,
	eBF_Src1_Color       = 15,
	eBF_Inv_Src1_Color   = 16,
	eBF_Src1_Alpha       = 17,
	eBF_Inv_Src1_Alpha   = 18
};

enum eBlendOp
{
	eBO_Add          = 1 ,
	eBO_Subtract     = 2 ,
	eBO_Rev_Subtract = 3 ,
	eBO_Min          = 4 ,
	eBO_Max          = 5
};

struct RenderTargetBlendDesc
{
	bool		 blendEnable;
	eBlendFactor srcBlend;
	eBlendFactor destBlend;
	eBlendOp	 blendOp;
	eBlendFactor srcBlendAlpha;
	eBlendFactor destBlendAlpha;
	eBlendOp	 blendOpAlpha;
	suInt8		 renderTargetWriteMask;
};

struct BlendStateDesc
{
	bool alphaToCoverageEnable  = false;
	bool independentBlendEnable = false;
	RenderTargetBlendDesc renderTarget[8];
};

class BlendState
{
public:
	BlendState();
	~BlendState();
	void ComputeHash();
	BlendStateDesc& GetDesc();

private:
	BlendStateDesc desc;
	ID3D11BlendState* pBlendState;
};

enum eFillMode
{
	eFM_Wireframe = 2 ,
	eFM_Solid     = 3 ,
};

enum eCullMode
{
	eCM_None  = 1,
	eCM_Front = 2,
	eCM_Back  = 3,
};

struct RasterizerStateDesc
{
	eFillMode fillMode;
	eCullMode cullMode;
	// determines if a triangle is front-or-back-facing.
	// if this parameter is True a triangle 
	// will be considered front-facing if its vertices are counter-clockwise on the render-target and
	// considered back-facing if they are clockwise.
	bool	  frontCounterClockwise;
	// depth value added to a given pixel
	sInt32	  depthBias;
	// maximum depth bias of a pixel
	Float	  depthBiasClamp;
	// scalar on given pixel's slope.
	Float	  slopeScaledDepthBias;
	// enable clipped base on distace.
	bool	  depthClipEnable;
	// enable scissor-rectangle culling. all pixel outside an active scissor rectangle are culled.
	bool	  scissorEnable;
	// specifies whether to use the quadrilateral or alpha line anti-aliasing algorithm on multisampling antialiasing(MSAA) render target.
	// set to true to use the quadrilateral line anti-aliasing algorithm and to False to use the alpha line anti-aliasing algorithm.
	bool      multisampleEnable;
	// specifies whether to enable line antialiasing
	// only applies if doing line drawing and multisampleEnable is False
	bool      antialiasedLineEnable;
};

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();
	void ComputeHash();
	RasterizerStateDesc& GetDesc();
private:
	RasterizerStateDesc desc;
	ID3D11RasterizerState* pRasterizerState;
};

enum eDepthWriteMask
{
	eDWM_Zero = 0 ,
	eDWM_All  = 1
};

enum eComparisonFunc
{
	eCF_Never         = 1 ,
	eCF_Less          = 2 ,
	eCF_Equal         = 3 ,
	eCF_Less_Equal    = 4 ,
	eCF_Greater       = 5 ,
	eCF_Not_Equal     = 6 ,
	eCF_Greater_Equal = 7 ,
	eCF_Always        = 8 ,
};

enum eStencilOp
{
	eSO_Keep     = 1,
	eSO_Zero     = 2,
	eSO_Replace  = 3,
	eSO_Incr_Sat = 4,
	eSO_Decr_Sat = 5,
	eSO_Invert   = 6,
	eSO_Incr     = 7,
	eSO_Desc     = 8,
};

struct DepthStencilOp
{
	eStencilOp		stencilFailOp;
	eStencilOp		stencilDepthFailOp;
	eStencilOp		stencilPassOp;
	eComparisonFunc stencilFunc;
};

struct DepthStencilDesc
{
	bool			depthEnable;
	eDepthWriteMask depthWriteMask;
	eComparisonFunc depthFunc;
	bool			stencilEnable;
	suInt8			stencilReadMask;
	suInt8			stencilWriteMask;
	DepthStencilOp	frontFace;
	DepthStencilOp	backFace;
};

class DepthStencilState
{
public:
	DepthStencilState();
	DepthStencilDesc& GetDesc();
private:
	ID3D11DepthStencilState * pDepthStencilState;
	DepthStencilDesc desc;
};

class RenderState : public SRefCounter
{
public:
	BlendStateDesc&		 GetBlendState();
	RasterizerStateDesc& GetRasterizerState();
	DepthStencilDesc&	 GetDepthStencilDesc();

public:
	BlendState        m_BlendState;
	RasterizerState   m_RasterizerState;
	DepthStencilState m_DepthStencilState;
};

typedef SSmartPointer<RenderState> renderStatePtr;