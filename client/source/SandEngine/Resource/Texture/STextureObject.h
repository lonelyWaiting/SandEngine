#pragma once

#include "SandEngine/Resource/SResource.h"
#include "SandEngine/Resource/SResourceManager.h"
#include "SandBase/Object/SSmartPointer.h"

enum eTextureView
{
	eTV_None = 0,
	eTV_RenderTarget = SBIT( 0 ) , 
	eTV_DepthStencil = SBIT( 1 ) ,
};

typedef SSmartPointer<ID3D11Texture2D> Texture2DPtr;

class STextureObject : public SResource
{
public:
	STextureObject( const char* filename , SResourceManager& manager );	
	STextureObject();
	void Ensureloaded();
	void SetD3DTexture(ID3D11Texture2D* tex);
	ID3D11Texture2D* GetD3DTexture();

protected:
	Texture2DPtr m_Texture;
};
typedef SSmartPointer<STextureObject> STextureObjectPtr;

struct SRenderableConfig
{
	bool enableAsRenderTarget = false;
	bool enableAsDepth        = false;
	int  width  = 0;
	int  height = 0;
};

typedef SSmartPointer<ID3D11RenderTargetView>	RenderTargetPtr;
typedef SSmartPointer<ID3D11DepthStencilView>	DepthStencilPtr;
class SRenderableTexture : public STextureObject
{
public:
	SRenderableTexture(const SRenderableConfig& cfg);
	void SetRenderableConfig(const SRenderableConfig& cfg);
	void Ensureloaded();
	void EnsureUnloaded();
	
	ID3D11RenderTargetView* GetD3DRenderTarget();
	ID3D11DepthStencilView* GetD3DDepthStencil();

protected:
	bool IsLoaded();
	void DoLoaded();
	void DoUnloaded();

protected:
	SRenderableConfig m_cfg;
	RenderTargetPtr m_renderTarget;
	DepthStencilPtr m_depthStencil;
};

typedef SSmartPointer<SRenderableTexture> SRenderableTexturePtr;