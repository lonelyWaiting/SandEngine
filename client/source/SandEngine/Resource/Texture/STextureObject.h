#pragma once
#include "SandEngine/SApidef.h"
#include "SandEngine/Resource/SResource.h"
#include "SandEngine/Resource/SResourceManager.h"
#include "SandBase/Object/SSmartPointer.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SVector4f.h"

typedef SSmartPointer<ID3D11Texture2D>			Texture2DPtr;
typedef SSmartPointer<ID3D11ShaderResourceView> ShaderResourceViewPtr;
typedef SSmartPointer<ID3D11SamplerState>		SamplerStatePtr;

enum SAND_API TextureWrapMode
{
	Repeat     = 0,
	Clamp      = 1,
	Mirror     = 2,
	MirrorOnce = 3,
	Border	   = 4
};

enum SAND_API TextureFilterMode
{
	Point      = 0,
	Bilinear   = 1,
	Trilinear  = 2,
	Anisotropy = 3
};

enum SAND_API TextureFormat
{
	STF_Unknow,
	STF_R8G8B8A8,
	STF_RGBAFloat,
	STF_RGBAHalf,
	STF_D24_S8
};

enum SAND_API TextureSampleComparison
{
	Never         = 0,
	Less          = 1,
	Equal         = 2,
	Less_Equal    = 3,
	Greater       = 4,
	Not_Equal     = 5,
	Greate_Equal  = 6,
	Always        = 7
};

class STexture : public SResource
{
public:
	STexture() {}

public:
	SAND_API int GetWidth()
	{
		return m_width;
	}
	SAND_API int GetHeight()
	{
		return m_height;
	}
	SAND_API bool IsUseMip()
	{
		return m_usemip;
	}
	SAND_API TextureWrapMode GetWrapMode()
	{
		return m_wrapMode;
	}
	SAND_API TextureFilterMode GetFilterMode()
	{
		return m_filterMode;
	}
	SAND_API const SVector4f& GetBorderColor()
	{
		return m_BorderColor;
	}
	SAND_API int GetMaxAnisotropy()
	{
		return m_MaxAnisotropy;
	}
	SAND_API int GetMipMapCount()
	{
		return m_mipNum;
	}
	SAND_API void EnableUseMip(bool enable)
	{
		m_usemip = enable;
	}
	SAND_API void SetWrapMode(TextureWrapMode mode)
	{
		m_wrapMode = mode;
	}
	SAND_API void SetFilterMode(TextureFilterMode mode)
	{
		m_filterMode = mode;
	}
	SAND_API void SetBorderColor(const SVector4f& c)
	{
		m_BorderColor = c;
	}
	SAND_API void SetMaxAnisotropy(int maxAnisotropy)
	{
		m_MaxAnisotropy = maxAnisotropy;
	}

public:
	int                    m_width         = 0;
	int                    m_height        = 0;
	bool                   m_usemip        = false;
	float                  m_mipMapBias    = 0.0f;
	TextureWrapMode        m_wrapMode      = TextureWrapMode::Repeat;
	TextureFilterMode      m_filterMode    = TextureFilterMode::Bilinear;
	TextureFormat	       m_format        = TextureFormat::STF_R8G8B8A8;
	TextureSampleComparison m_Comparision  = TextureSampleComparison::Always;
	SVector4f			   m_BorderColor   = SVector4f::Zero();
	int					   m_MaxAnisotropy = 0;
	bool                   m_isFloatFormat = false;
	int                    m_componentNum  = 4;
	int					   m_mipNum        = 1;
	Texture2DPtr	       m_Texture;

};

class STexture2D : public STexture
{
public:
	SAND_API STexture2D(int width, int height);
	SAND_API STexture2D(int width, int height, TextureFormat fomat, bool mipChain);
	~STexture2D();

	SAND_API ID3D11SamplerState* GetSampler()	
	{
		return m_SamplerState;
	}
	SAND_API ID3D11ShaderResourceView* GetSRV()
	{
		return m_SRV;
	}

	SAND_API SVector4f			GetPixel(int x, int y);
	SAND_API void				SetPixel(int x, int y, float r, float g, float b, float a, int miplevel = 0);
	SAND_API void				SetPixel(int x, int y, const SVector3f& c, int miplevel = 0);
	SAND_API void				SetPixel(int x, int y, const SVector4f& c, int miplevel = 0);
	SAND_API void				Apply();
	
protected:
	void*                 m_systemMem    = nullptr;
	ShaderResourceViewPtr m_SRV          = nullptr;
	SamplerStatePtr       m_SamplerState = nullptr;
};
typedef SSmartPointer<STexture2D> STexture2DPtr;

typedef SSmartPointer<ID3D11RenderTargetView> RenderTargetViewPtr;
typedef SSmartPointer<ID3D11DepthStencilView> DepthStencilViewPtr;
class SRenderTexture : public STexture
{
public:
	SAND_API SRenderTexture(int width, int height, int depth, TextureFormat format);
	SAND_API SRenderTexture(ID3D11Texture2D* tex);

public:
	SAND_API void Create();
	SAND_API ID3D11RenderTargetView* GetRenderTargetView() { return m_RTV; }
	SAND_API ID3D11DepthStencilView* GetDepthStencilView() { return m_DSV; }

public:
	SAND_API bool AllowGenerateMip()			 { return m_autoGenerateMip; }
	SAND_API void EnableGenerateMip(bool enable) { m_autoGenerateMip = enable; }

protected:
	bool m_autoGenerateMip = false;
	RenderTargetViewPtr m_RTV;
	DepthStencilViewPtr m_DSV;
	Texture2DPtr        m_depthTex;
};
typedef SSmartPointer<SRenderTexture> SRenderTexturePtr;