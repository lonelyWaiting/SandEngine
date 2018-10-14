#include "SandEnginePCH.h"
#include "STextureManager.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandBase/IO/SFileStream.h"
#include "DDSTextureLoader.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

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
	tex2D->Apply();

	SAFE_DELETE_ARRAY(data);

	return tex2D;
}