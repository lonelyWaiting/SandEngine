#include "SandEnginePCH.h"
#include "STextureManager.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandBase/IO/SFileStream.h"
#include "DDSTextureLoader.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

#include "loadpng/lodepng.h"
#include "SandEngine/Resource/Image/SImage.h"

static SResourceManager gTextureManager;

STexture2D* STextureManager::Load2DTexturePNG(const char* filename)
{
	std::vector<unsigned char> pngData;
	unsigned w, h;
	unsigned error = lodepng::decode(pngData, w, h, filename);

	STexture2D* tex2D = new STexture2D(w, h, TextureFormat::STF_R8G8B8A8, false);

	for (int i = 0; i < (int)w; i++)
	{
		for (int j = 0; j < (int)h; j++)
		{
			int offset = 4 * (j * w + i);
			tex2D->SetPixel(i, j, SVector4f(pngData[offset] / 255.0f, pngData[offset + 1] / 255.0f, pngData[offset + 2] / 255.0f, pngData[offset + 3] / 255.0f));
		}
	}
	tex2D->SetFilterMode(TextureFilterMode::Anisotropy);
	tex2D->SetMaxAnisotropy(1);
	tex2D->Apply();

	return tex2D;
}

STexture2D * STextureManager::Load2DTextureFromFile( const char * filename )
{
	unsigned char* data = nullptr;
	int   size = 0;

	SFileInStream fi;
	if (fi.OpenFile(filename))
	{
		size = fi.GetFileSize();
		data = new unsigned char[size];
		fi.Read((char*)data, size);
		fi.Close();
	}
	
	ID3D11Resource*				resource = nullptr;
	ID3D11ShaderResourceView*	srv		 = nullptr;
	DirectX::CreateDDSTextureFromMemory(SRenderHelper::g_Device, data, size, &resource, &srv);

	STexture2D* tex2D = new STexture2D();
	tex2D->SetResourceAndSRV(resource, srv);
	tex2D->SetFilterMode(TextureFilterMode::Anisotropy);
	tex2D->SetMaxAnisotropy(1);
	tex2D->Apply();

	SAFE_DELETE_ARRAY(data);

	return tex2D;
}

STexture2D* STextureManager::GetBlackTexture()
{
	static STexture2D tex(4, 4, TextureFormat::STF_R8G8B8A8, false);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tex.SetPixel(j, i, SVector4f(0.0f, 0.0f, 0.0f, 0.0f), 0);
		}
	}
	tex.Apply();

	return &tex;
}

STexture2D* STextureManager::GetWhiteTexture()
{
	static STexture2D tex(4, 4, TextureFormat::STF_R8G8B8A8, false);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tex.SetPixel(j, i, SVector4f(1.0f, 1.0f, 1.0f, 1.0f), 0);
		}
	}
	tex.Apply();

	return &tex;
}

STexture2D* STextureManager::GetFlatNormal()
{
	static STexture2D tex(4, 4, TextureFormat::STF_R8G8B8A8, false);

	SVector4f flatNormal(0.5f, 0.5f, 0.5f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tex.SetPixel(j, i, flatNormal, 0);
		}
	}

	tex.Apply();

	return &tex;
}