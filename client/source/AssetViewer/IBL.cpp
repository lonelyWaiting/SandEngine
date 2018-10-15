#include "IBL.h"
#include "SandBase/Math/SMath.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SVector2f.h"
#include "SandEngine/Resource/Image/SImage.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandEngine/Callback/SCallback.h"
#include "SandEngine/Application/SandEngineModule.h"
#include "SandEngine/Resource/Shaders/SShader.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandEngine/Resource/SStaticMeshManager.h"
#include "SandBase/Math/SMatrix4f.h"
#include "SandEngine/Resource/SBuffer.h"

SVector2f CartesianToSpherical(const SVector3f& dir)
{
	float theta = SMath::Acos(SMath::clamp(dir.y, -1.0f, 1.0f));
	float phi   = SMath::ATan2(dir.z, dir.x);
	phi = phi < 0.0f ? phi + SMath::TWO_PI : phi;
	return SVector2f(phi / SMath::TWO_PI, theta / SMath::PI);
}

static STexture2DPtr sIrradianceMap = nullptr;
static STexture2DPtr sBRDFLutMap    = nullptr;
static STexture2DPtr sPrefilterMap  = nullptr;
static SShader		 sIBLShader;
static SShader		 sPBRShader;

struct cbIBL
{
	SMatrix4f worldMatrix;
	float roughness;
	float metallic;
	SVector2f padding;
};

struct PointLight
{
	SVector3f lightPos;
	float padding;
	SVector3f lightColor;
	float padding2;
};

struct cbLight
{
	PointLight light[3];
};

static SBuffer* sCBIbl          = nullptr;
static SBuffer* sDirectionLight = nullptr;

struct Entities
{
	SStaticMesh*	staticMesh;
	SVector3f		position;
};
Entities entities[49];

void InitCB()
{
	sCBIbl          = new SBuffer(eBU_Dynamic, sizeof(cbIBL), 1, nullptr, eBF_Constant);
	sDirectionLight = new SBuffer(eBU_Dynamic, sizeof(cbLight), 1, nullptr, eBF_Constant);
}

void InitPrecomputeMap()
{
	SImageDecode decoder;
	decoder.Load("../data/textures/brdfLut.hdr");

	sBRDFLutMap = new STexture2D(decoder.GetWidth(), decoder.GetHeight(), TextureFormat::STF_RGBAFloat, false);
	for (int i = 0; i < decoder.GetWidth(); i++)
	{
		for(int j = 0; j < decoder.GetHeight(); j++)
		{
			sBRDFLutMap->SetPixel(i, j, decoder.GetPixel(i, j));
		}
	}
	sBRDFLutMap->Apply();

	decoder.Load("../data/textures/Irradiance.hdr");
	sIrradianceMap = new STexture2D(decoder.GetWidth(), decoder.GetHeight(), TextureFormat::STF_RGBAFloat, false);
	for (int i = 0; i < decoder.GetWidth(); i++)
	{
		for (int j = 0; j < decoder.GetHeight(); j++)
		{
			sIrradianceMap->SetPixel(i, j, decoder.GetPixel(i, j));
		}
	}
	sIrradianceMap->Apply();

	decoder.Load("../data/textures/prefilter0.hdr");
	int width = decoder.GetWidth(), height = decoder.GetHeight();
	sPrefilterMap = new STexture2D(width, height, TextureFormat::STF_RGBAFloat, true);

	int mipNum = sPrefilterMap->GetMipMapCount();

	for (int miplevel = 0; miplevel < mipNum; miplevel++)
	{
		if (miplevel != 0)
		{
			SString path = "../data/textures/prefilter";
			path.AppendFormat("%d.hdr", miplevel);
			decoder.Load(path.AsChar());
		}

		for (int i = 0; i < decoder.GetWidth(); i++)
		{
			for (int j = 0; j < decoder.GetHeight(); j++)
			{
				sPrefilterMap->SetPixel(i, j, decoder.GetPixel(i, j), miplevel);
			}
		}
	}
	sPrefilterMap->SetFilterMode(Trilinear);
	sPrefilterMap->Apply();
}

class IBLHandler : public SCallbackHandle
{
public:
	virtual void OnHandle(const SCallbackUserData& userData)
	{
		if (userData.pSender == &SandEngine::Callback.OnEngineInit)
		{
			sIBLShader.Load("../data/shaders/debugTexture.hlsl", nullptr, "ps_main");
			sPBRShader.Load("../data/shaders/pbrIBL.hlsl", "vs_main", "ps_main");

			SStaticMesh* staticMesh = SStaticMeshMangaer::LoadStaticMesh("..\\asset\\models\\test.fbx");
			/*for (int i = 0; i < 10; i++)
			{
				entities[i].staticMesh = staticMesh;
				entities[i].position = SVector3f(-200.0f + 40.0f * i, 0, 100.0f);
			}*/

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					entities[j * 7 + i].staticMesh = staticMesh;
					entities[j * 7 + i].position = SVector3f(-100.0f + 30.0f * i, -100.0f + 30.0f * j, 100.0f);
				}
			}

			InitPrecomputeMap();
			InitCB();
		}
		else if (userData.pSender == &SandEngine::Callback.OnBeginRender)
		{
			SRenderHelper::BindTexture(eST_Pixel, 5, sBRDFLutMap);
			SRenderHelper::BindTexture(eST_Pixel, 6, sIrradianceMap);
			SRenderHelper::BindTexture(eST_Pixel, 7, sPrefilterMap);

			for (int i = 0; i < 7; i++)
			{
				for (int j = 0; j < 7; j++)
				{
					D3D11_MAPPED_SUBRESOURCE data;
					SRenderHelper::g_ImmediateContext->Map(sCBIbl->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
					cbIBL& ibl = *(cbIBL*)(data.pData);
					ibl.worldMatrix.MakeIdentity();
					ibl.worldMatrix.SetTranslate(entities[j * 7 + i].position);
					ibl.worldMatrix.MakeTranspose();

					ibl.roughness = SMath::clamp((7 - i) / 7.0f, 0.05f, 1.0f);
					ibl.metallic = (7 - i) / 7.0f;

					SRenderHelper::g_ImmediateContext->Unmap(sCBIbl->GetBuffer(), 0);

					D3D11_MAPPED_SUBRESOURCE lightData;
					SRenderHelper::g_ImmediateContext->Map(sDirectionLight->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightData);
					cbLight& l = *(cbLight*)lightData.pData;

					l.light[0].lightPos = SVector3f(0.0f, 100.0f, 100.0f);
					l.light[0].lightColor = SVector3f(1.0f, 1.0f, 1.0f);

					l.light[1].lightPos = SVector3f(0.0f, -100.0f, -100.0f);
					l.light[1].lightColor = SVector3f(1.0f, 0.8f, 1.0f);

					l.light[2].lightPos = SVector3f(100.0f, 0.0f, 300);
					l.light[2].lightColor = SVector3f(1.0f, 1.0f, 0.5f);
					SRenderHelper::g_ImmediateContext->Unmap(sDirectionLight->GetBuffer(), 0);

					ID3D11Buffer* vsCB = sCBIbl->GetBuffer();
					SRenderHelper::g_ImmediateContext->VSSetConstantBuffers(1, 1, &vsCB);
					ID3D11Buffer* psCB = sDirectionLight->GetBuffer();
					SRenderHelper::g_ImmediateContext->PSSetConstantBuffers(2, 1, &psCB);

					SRenderHelper::RenderStaticMesh(*(entities[j * 7 + i].staticMesh), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sPBRShader);
				}
			}
		}
	}
}g_IBLHandler;

void IBLInit()
{
	SandEngine::Callback.OnBeginRender  += g_IBLHandler;
	SandEngine::Callback.OnEngineInit   += g_IBLHandler;
}

void IBLDeInit()
{
	SandEngine::Callback.OnBeginRender -= g_IBLHandler;
	SandEngine::Callback.OnEngineInit  -= g_IBLHandler;
}

float RadicalInverse_VdC(unsigned int bits)
{
	bits = (bits << 16) | (bits >> 16);
	bits = ((bits & 0x55555555u) << 1) | ((bits & 0xAAAAAAAAu) >> 1);
	bits = ((bits & 0x33333333u) << 2) | ((bits & 0xCCCCCCCCu) >> 2);
	bits = ((bits & 0x0F0F0F0Fu) << 4) | ((bits & 0xF0F0F0F0u) >> 4);
	bits = ((bits & 0x00FF00FFu) << 8) | ((bits & 0xFF00FF00u) >> 8);
	return (float)bits * 2.3283064365386963e-10f; // / 0x100000000
}

// ----------------------------------------------------------------------------
SVector2f Hammersley(unsigned int i, unsigned int N)
{
	return SVector2f(i / (float)N, RadicalInverse_VdC(i));
}

SVector3f ImportanceSampleGGX(SVector2f Xi, float Roughness, SVector3f N)
{
	float a = Roughness * Roughness;

	float Phi = 2 * SMath::PI * Xi.x;
	float cosTheta = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	SVector3f H = SVector3f(0.0f, 0.0f, 0.0f);
	H.x = sinTheta * cos(Phi);
	H.y = cosTheta;
	H.z = sinTheta * sin(Phi);

	SVector3f front = abs(N.z) < 0.999f ? SVector3f(0.0f, 0.0f, 1.0f) : SVector3f(1.0f, 0.0f, 0.0f);
	SVector3f right = SVector3f::Normalize(SVector3f::cross(N, front));
	front = SVector3f::cross(right, N);

	return SVector3f::Normalize(right * H.x + N * H.y + front * H.z);
}

float G_Smith(float roughness, float NdotV, float NdotL)
{
	NdotV = SMath::clamp(NdotV, 0.0f, 1.0f);
	NdotL = SMath::clamp(NdotL, 0.0f, 1.0f);

	//float k    = (roughness + 1) * (roughness + 1) / 8;
	float k    = roughness * roughness / 2;
	float ggx1 = NdotV / (NdotV * (1 - k) + k);
	float ggx2 = NdotL / (NdotL * (1 - k) + k);
	return ggx1 * ggx2;
}

SVector2f IntegrateBRDF(float roughness, float NdotV)
{
	SVector3f V;
	V.x = sqrt(1.0f - NdotV * NdotV);
	V.y = NdotV;
	V.z = 0.0F;

	float A = 0, B = 0;

	SVector3f N = SVector3f(0, 1, 0);

	unsigned int NumSamples = 1024;
	for (unsigned int i = 0; i < NumSamples; i++)
	{
		SVector2f Xi = Hammersley(i, NumSamples);
		SVector3f H = ImportanceSampleGGX(Xi, roughness, N);
		SVector3f L = SVector3f::Normalize(2 * dot(V, H) * H - V);

		float NdotL = SMath::clamp(L.y, 0.0f, 1.0f);
		float NdotH = SMath::clamp(H.y, 0.0f, 1.0f);
		float VdotH = SMath::clamp(dot(V, H), 0.0f, 1.0f);

		if (NdotL > 0.0f)
		{
			float G = G_Smith(roughness, NdotV, NdotL);
			float G_Vis = G * VdotH / (NdotH * NdotV);
			float Fc = pow(1 - VdotH, 5);
			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}

	return SVector2f(A / NumSamples, B / NumSamples);
}

void IBLGenerateLutMap(const char* path)
{
	SImageDecode image;
	image.Load(path);

	sBRDFLutMap = new STexture2D(image.GetWidth(), image.GetHeight(), STF_RGBAFloat, false);

	SImageEncode encoder(image.GetWidth(), image.GetHeight());

	int width  = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < width; i++)
	{
		std::cout << "generate brdf lut map: %.2f" << (float)i / (float)width << "\r";

		for (int j = 0; j < height; j++)
		{
			float NdotV     = i / (float)(width - 1);
			float roughness = j / (float)(height - 1);
			SVector2f brdf  = IntegrateBRDF(roughness, NdotV);

			sBRDFLutMap->SetPixel(i, height - 1 - j, brdf.x, brdf.y, 0.0f, 1.0f);
			encoder.SetPixel(i, height - 1 - j, brdf.x, brdf.y, 0.0f, 1.0f);
		}
	}

	sBRDFLutMap->Apply();
	encoder.write("../data/textures/brdfLut.hdr");
}

void IBLGenerateIrradianceMap(const char* path)
{
	SImageDecode image;
	image.Load(path);

	sIrradianceMap = new STexture2D(image.GetWidth(), image.GetHeight(), STF_RGBAFloat, false);
	SImageEncode encode(image.GetWidth(), image.GetHeight());

	int   phiSampleCount = 100;
	int   thetaSampleCount = 100;
	float phiDelta   = SMath::TWO_PI / phiSampleCount;
	float thetaDelta = SMath::PI_DIV_2 / thetaSampleCount;

	int width  = image.GetWidth();
	int height = image.GetHeight();

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			float phi   = SMath::TWO_PI * i / (float)width;
			float theta = SMath::PI * j / (float)height;

			SVector3f up = SVector3f(SMath::SinRad(theta) * SMath::CosRad(phi), SMath::CosRad(theta), SMath::SinRad(theta) * SMath::SinRad(phi));
			SVector3f front = up.z < 0.999f ? SVector3f(0.0f, 0.0f, 1.0f) : SVector3f(0.0f, -1.0f, 0.0f);
			SVector3f right = SVector3f::Normalize(SVector3f::cross(up, front));
			front = SVector3f::Normalize(SVector3f::cross(right, up));

			SVector3f irradiance = SVector3f::Zero();

			for (float localPhi = 0.0f; localPhi < SMath::TWO_PI; localPhi += phiDelta)
			{
				for (float localTheta = 0.0f; localTheta < SMath::PI_DIV_2; localTheta += thetaDelta)
				{
					SVector3f localDir = SVector3f(SMath::SinRad(localTheta) * SMath::CosRad(localPhi), SMath::CosRad(localTheta), SMath::SinRad(localTheta) * SMath::SinRad(localPhi));
					SVector3f dir = right * localDir.x + up * localDir.y + front * localDir.z;
					dir.Normalize();

					SVector2f uv = CartesianToSpherical(dir);
					irradiance += image.GetPixel(int(uv.x * (width - 1)), int(uv.y * (height - 1))).xyz() * SMath::CosRad(localTheta) * SMath::SinRad(localTheta);
				}
			}

			irradiance = SMath::PI * irradiance / (float)(phiSampleCount * thetaSampleCount);

			sIrradianceMap->SetPixel(i, j, irradiance);
			encode.SetPixel(i, j, irradiance);
		}
	}

	sIrradianceMap->Apply();
	encode.write("../data/textures/Irradiance.hdr");
}

void IBLGeneratePrefilterMap(const char* path)
{
	SImageDecode image;
	image.Load(path);

	sPrefilterMap = new STexture2D(image.GetWidth(), image.GetHeight(), STF_RGBAFloat, true);

	int width  = image.GetWidth();
	int height = image.GetHeight();
	int mipNum = sPrefilterMap->GetMipMapCount();
	unsigned int sampleNum = 1024;

	for (int miplevel = 0; miplevel < mipNum; miplevel++)
	{
		float roughness = (float)miplevel / (float)(mipNum - 1);

		int cur_width  = width >> miplevel;
		int cur_height = height >> miplevel;

		SImageEncode encode(cur_width, cur_height);

#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < cur_width; i++)
		{
			for (int j = 0; j < cur_height; j++)
			{
				float phi   = (float)i / (float)(cur_width - 1) * SMath::TWO_PI;
				float theta = (float)j / (float)(cur_height - 1) * SMath::PI;

				SVector3f N(SMath::SinRad(theta) * SMath::CosRad(phi), SMath::CosRad(theta), SMath::SinRad(theta) * SMath::SinRad(phi));
				N.Normalize();
				SVector3f V = N;

				SVector3f prefilterdColor = SVector3f::Zero();
				float sumWeight = 0.0f;

				for (unsigned int isample = 0; isample < sampleNum; isample++)
				{
					SVector2f Xi = Hammersley(isample, sampleNum);
					SVector3f H = ImportanceSampleGGX(Xi, roughness, N);

					SVector3f L = SVector3f::Normalize(H * 2.0f * dot(V, H) - V);

					float NdotL = dot(N, L);
					if (NdotL > 0.0f)
					{
						SVector2f sphericalCoordinate = CartesianToSpherical(L);
						SVector2f uv(sphericalCoordinate.x * (width - 1), sphericalCoordinate.y * (height - 1));

						prefilterdColor += image.GetPixel((int)uv.x, (int)uv.y).xyz() * NdotL;
						sumWeight += NdotL;
					}
				}

				prefilterdColor /= sumWeight;
				sPrefilterMap->SetPixel(i, j, prefilterdColor, miplevel);
				encode.SetPixel(i, j, prefilterdColor);
			}
		}

		SString path = "../data/textures/prefilter";
		path.AppendFormat("%d.hdr", miplevel);
		encode.write(path.AsChar());
	}

	sPrefilterMap->Apply();
}