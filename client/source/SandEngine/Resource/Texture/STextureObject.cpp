#include "SandEnginePCH.h"
#include "STextureObject.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

DXGI_FORMAT ConvertToDxFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::STF_R8G8B8A8:
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case TextureFormat::STF_RGBAFloat:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case TextureFormat::STF_RGBAHalf:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case TextureFormat::STF_D24_S8:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	}

	return DXGI_FORMAT_UNKNOWN;
}

TextureFormat ConvertToNativeFormat(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return STF_R8G8B8A8;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return STF_RGBAFloat;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return STF_RGBAHalf;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		return STF_D24_S8;
	default:
		return STF_Unknow;
	}
}
int GetFormatComponentCount(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::STF_R8G8B8A8:
		return 4;
	case TextureFormat::STF_RGBAFloat:
		return 4;
	case TextureFormat::STF_RGBAHalf:
		return 4;
	case TextureFormat::STF_D24_S8:
		return 4;
	}

	return 1;
}

bool IsFloatFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::STF_RGBAFloat:
	case TextureFormat::STF_RGBAHalf:
	case TextureFormat::STF_D24_S8:
		return true;
	case TextureFormat::STF_R8G8B8A8:
		return false;
	}

	return false;
}

D3D11_FILTER ConvertToDxFilter(TextureFilterMode mode)
{
	switch (mode)
	{
	case Point:
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case Bilinear:
		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case Trilinear:
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case Anisotropy:
		return D3D11_FILTER_ANISOTROPIC;
	}

	return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
}

D3D11_TEXTURE_ADDRESS_MODE ConvertToDxAddressMode(TextureWrapMode mode)
{
	switch (mode)
	{
	case Repeat:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case Clamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case Mirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case MirrorOnce:
		return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
	case Border:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}

	return D3D11_TEXTURE_ADDRESS_WRAP;
}

D3D11_COMPARISON_FUNC ConvertToDxComparisonFunc(TextureSampleComparison comparsion)
{
	switch (comparsion)
	{
	case Never:
		return D3D11_COMPARISON_NEVER;
	case Less:
		return D3D11_COMPARISON_LESS;
	case Equal:
		return D3D11_COMPARISON_EQUAL;
	case Less_Equal:
		return D3D11_COMPARISON_LESS_EQUAL;
	case Greater:
		return D3D11_COMPARISON_GREATER;
	case Not_Equal:
		return D3D11_COMPARISON_NOT_EQUAL;
	case Greate_Equal:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case Always:
		return D3D11_COMPARISON_ALWAYS;
	}

	return D3D11_COMPARISON_ALWAYS;
}

STexture2D::~STexture2D()
{
	SAFE_DELETE_ARRAY(m_systemMem);
}

STexture2D::STexture2D(int width, int height)
{
	SAFE_DELETE_ARRAY(m_systemMem);

	m_width     = width;
	m_height    = height;
	m_format    = STF_R8G8B8A8;
	m_systemMem = new unsigned char[m_width * m_height * 4 * sizeof(unsigned char)];
	memset(m_systemMem, 0, width * height * 4 * sizeof(float));
}

STexture2D::STexture2D(int width, int height, TextureFormat format, bool mipChain)
{
	SAFE_DELETE_ARRAY(m_systemMem);

	m_width  = width;
	m_height = height;
	m_format = format;
	m_usemip = mipChain;
	m_mipNum = m_usemip ? 0 : 1;

	int elementNum = width * height;
	if (m_usemip)
	{
		int minSize = SMath::Min(m_width, m_height);
		while (minSize != 0) { m_mipNum++; minSize >>= 1; }
		for (int i = 1; i < m_mipNum; i++)	elementNum += width * height >> (2 * i);
	}

	m_componentNum  = GetFormatComponentCount(format);
	m_isFloatFormat = IsFloatFormat(format);
	if (m_isFloatFormat)
	{
		m_systemMem = new float[elementNum * m_componentNum];
		memset(m_systemMem, 0, elementNum * m_componentNum * sizeof(float));
	}
	else
	{
		m_systemMem = new unsigned char[elementNum * m_componentNum];
		memset(m_systemMem, 0, elementNum * m_componentNum * sizeof(unsigned char));
	}
}

SVector4f STexture2D::GetPixel(int x, int y)
{
	if (m_isFloatFormat)
	{
		float* data = (float*)m_systemMem;
		int offset  = (y * m_width + x) * m_componentNum;

		switch (m_componentNum)
		{
		case 4:
			return SVector4f(data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
		case 3:
			return SVector4f(data[offset], data[offset + 1], data[offset + 2], 1.0f);
		case 2:
			return SVector4f(data[offset], data[offset + 1], 0.0f, 1.0f);
		case 1:
			return SVector4f(data[offset], 0.0f, 0.0f, 1.0f);
		}
	}
	else
	{
		unsigned char* data = (unsigned char*)m_systemMem;
		int offset = (y * m_width + x) *  m_componentNum;

		switch (m_componentNum)
		{
		case 4:
			return SVector4f(data[offset] / 255.f, data[offset + 1] / 255.f, data[offset + 2] / 255.f, data[offset + 3] / 255.f);
		case 3:
			return SVector4f(data[offset] / 255.f, data[offset + 1] / 255.f, data[offset + 2] / 255.f, 1.0f);
		case 2:
			return SVector4f(data[offset] / 255.f, data[offset + 1] / 255.f, 0.0f, 1.0f);
		case 1:
			return SVector4f(data[offset] / 255.f, 0.0f, 0.0f, 1.0f);
		}
	}

	return SVector4f(0.0f, 0.0f, 0.0f, 0.0f);
}

void STexture2D::SetPixel(int x, int y, float r, float g, float b, float a, int miplevel /*= 0*/)
{
	if (miplevel < 0 || miplevel >= m_mipNum)	return;

	int cur_mip_width = m_width >> miplevel;
	int offset = 0;
	for (int i = 0; i <= miplevel - 1; i++)	offset += m_width * m_height >> (2 * i);

	float v[4] = { r,g,b,a };

	if (m_isFloatFormat)
	{
		float* data = (float*)m_systemMem + (offset + y * cur_mip_width + x)  * m_componentNum;
		for (int i = 0; i < m_componentNum; i++)	data[i] = v[i];
	}
	else
	{
		unsigned char* data = (unsigned char*)m_systemMem + (offset + y * cur_mip_width + x) * m_componentNum;
		for (int i = 0; i < m_componentNum; i++)	data[i] = (unsigned char)(v[i] * 255);
	}
}

void STexture2D::SetPixel(int x, int y, const SVector3f& c, int miplevel /*= 0*/)
{
	SetPixel(x, y, c.x, c.y, c.z, 1.0f, miplevel);
}

void STexture2D::SetPixel(int x, int y, const SVector4f& c, int miplevel /*= 0*/)
{
	SetPixel(x, y, c.x, c.y, c.z, c.w, miplevel);
}

void STexture2D::Apply()
{
	if (!m_Texture)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width              = m_width;
		desc.Height             = m_height;
		desc.MipLevels          = m_usemip ? m_mipNum : 1;
		desc.ArraySize          = 1;
		desc.Format             = ConvertToDxFormat(m_format);
		desc.Usage              = D3D11_USAGE_DEFAULT;
		desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags     = 0;
		desc.SampleDesc.Count   = 1;
		desc.SampleDesc.Quality = 0;
		desc.MiscFlags          = 0;
		
		SArray<D3D11_SUBRESOURCE_DATA> initDataArray;
		initDataArray.Reserve(m_mipNum);

		int elementSize = m_isFloatFormat ? sizeof(float) : sizeof(unsigned char);

		for (int i = 0, offset = 0, row_pitch = m_width * m_componentNum * elementSize; i < m_mipNum; i++)
		{
			initDataArray.PushBack(D3D11_SUBRESOURCE_DATA());
			D3D11_SUBRESOURCE_DATA& initData = initDataArray.Peek();
			initData.pSysMem          = (char*)m_systemMem + offset;
			initData.SysMemPitch      = row_pitch;
			initData.SysMemSlicePitch = 0;

			offset += row_pitch * m_height >> i;
			row_pitch >>= 1;
		}

		ID3D11Texture2D* tex2D = nullptr;
		SRenderHelper::g_Device->CreateTexture2D(&desc, initDataArray.GetData(), &tex2D);
		m_Texture = tex2D;
		if (tex2D) tex2D->Release();

		if (m_Texture)
		{
			const SString& _DebugName = GetFilename();
			m_Texture->SetPrivateData(WKPDID_D3DDebugObjectName, _DebugName.GetLength(), _DebugName.AsChar());
		}
	}

	if (!m_SRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format                    = ConvertToDxFormat(m_format);
		srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels       = m_mipNum;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* srv = nullptr;
		SRenderHelper::g_Device->CreateShaderResourceView(m_Texture, &srvDesc, &srv);
		m_SRV = srv;
		srv->Release();
	}

	if (!m_SamplerState)
	{
		D3D11_SAMPLER_DESC desc;
		desc.Filter         = ConvertToDxFilter(m_filterMode);
		desc.MaxAnisotropy  = m_MaxAnisotropy;
		desc.AddressU       = ConvertToDxAddressMode(m_wrapMode);
		desc.AddressV       = desc.AddressU;
		desc.AddressW       = desc.AddressU;
		desc.BorderColor[0] = m_BorderColor[0];
		desc.BorderColor[1] = m_BorderColor[1];
		desc.BorderColor[2] = m_BorderColor[2];
		desc.BorderColor[3] = m_BorderColor[3];
		desc.MinLOD         = 0.0f;
		desc.MaxLOD         = 0.0f;
		desc.MipLODBias     = 0.0f;
		desc.ComparisonFunc = ConvertToDxComparisonFunc(m_Comparision);
		ID3D11SamplerState* samplerState;
		SRenderHelper::g_Device->CreateSamplerState(&desc, &samplerState);
		m_SamplerState = samplerState;
	}
}

SRenderTexture::SRenderTexture(int width, int height, int depth, TextureFormat format)
{
	m_width  = width;
	m_height = height;
	m_format = format;
}

SRenderTexture::SRenderTexture(ID3D11Texture2D* tex)
{
	m_Texture = tex;
	D3D11_TEXTURE2D_DESC desc;
	m_Texture->GetDesc(&desc);
	m_width  = desc.Width;
	m_height = desc.Height;
	m_format = ConvertToNativeFormat(desc.Format);
}

void SRenderTexture::Create()
{
	if (m_Texture == nullptr)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width          = m_width;
		desc.Height         = m_height;
		desc.MipLevels      = m_usemip ? 0 : 1;
		desc.ArraySize      = 1;
		desc.Format         = ConvertToDxFormat(m_format);
		desc.Usage          = D3D11_USAGE_DEFAULT;
		desc.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags      = m_usemip && m_autoGenerateMip ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		ID3D11Texture2D* tex2D = nullptr;
		SRenderHelper::g_Device->CreateTexture2D(&desc, nullptr, &tex2D);
		m_Texture = tex2D;
	}

	SString _DebugName = GetFilename();
	m_Texture->SetPrivateData(WKPDID_D3DDebugObjectName, _DebugName.GetLength(), _DebugName.AsChar());

	if (m_RTV == nullptr)
	{
		D3D11_RENDER_TARGET_VIEW_DESC RTdesc;
		RTdesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
		RTdesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTdesc.Texture2D.MipSlice = 0;

		ID3D11RenderTargetView* rtView = nullptr;
		SRenderHelper::g_Device->CreateRenderTargetView(m_Texture, &RTdesc, &rtView);
		m_RTV = rtView;
		rtView->Release();
	}

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width              = m_width;
	dsd.Height             = m_height;
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
	ID3D11Texture2D*		pDepthStencilTex = nullptr;
	if (SRenderHelper::g_Device->CreateTexture2D(&dsd, 0, &pDepthStencilTex) != S_OK)	return;
	m_depthTex = pDepthStencilTex;
	int curRef = pDepthStencilTex->Release();

	_DebugName = GetFilename();
	m_depthTex->SetPrivateData(WKPDID_D3DDebugObjectName, _DebugName.GetLength(), _DebugName.AsChar());

	if (SRenderHelper::g_Device->CreateDepthStencilView(pDepthStencilTex, 0, &pDepthStencilView) != S_OK)	return;
	m_DSV      = pDepthStencilView;
	pDepthStencilView->Release();
}