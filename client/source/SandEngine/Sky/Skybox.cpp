#include "SandEnginePCH.h"
#include "Skybox.h"
#include "SandEngine/Resource/Image/SImage.h"
#include "SandBase/Math/SMatrix4f.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandEngine/Resource/Shaders/SShader.h"
#include "SandEngine/Application/SRenderer.h"

struct skyCB
{
	SMatrix4f cameraToWorld;
};

class Skybox
{
public:
	~Skybox();
	void LoadSkybox(const char* filename);
	void Render();

public:
	STexture2DPtr m_skybox;
	SBuffer*	  m_skyCB;
};

Skybox::~Skybox()
{
	SAFE_DELETE(m_skyCB);
}

void Skybox::LoadSkybox(const char * filename)
{
	SImageDecode decoder;
	decoder.Load(filename);

	m_skybox = new STexture2D(decoder.GetWidth(), decoder.GetHeight(), TextureFormat::STF_RGBAFloat, false);	
	for (int i = 0; i < decoder.GetWidth(); i++)
	{
		for (int j = 0; j < decoder.GetHeight(); j++)
		{
			m_skybox->SetPixel(i, j, decoder.GetPixel(i, j));
		}
	}
	m_skybox->Apply();
	m_skyCB = new SBuffer(eBU_Dynamic, sizeof(skyCB), 1, nullptr, eBF_Constant);
}

void Skybox::Render()
{
	if (m_skybox && m_skyCB)
	{
		ID3D11Buffer* cb = m_skyCB->GetBuffer();
		D3D11_MAPPED_SUBRESOURCE data;
		SRenderHelper::g_ImmediateContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		skyCB& sky = *(skyCB*)(data.pData);
		sky.cameraToWorld = SRenderer::Get().GetMainCamera().GetCameraToWorldMatrix();
		sky.cameraToWorld.MakeTranspose();
		SRenderHelper::g_ImmediateContext->Unmap(cb, 0);

		SRenderHelper::g_ImmediateContext->PSSetConstantBuffers(1, 1, &cb);
		SRenderHelper::BindTexture(eST_Pixel, 5, m_skybox);

		SRenderHelper::RenderFullScreen(SShader::FindShader("../data/shaders/skybox.hlsl"));
	}
}

Skybox g_skybox;

void SkyboxInit(const char* filename)
{
	g_skybox.LoadSkybox(filename);
}

void SkyboxRender()
{
	g_skybox.Render();
}