#include "SandEnginePCH.h"
#include "STextureManager.h"
#include "SandEngine/Resource/Texture/STextureObject.h"

STextureObject * STextureManager::Load2DTexture( const char * filename )
{
	return nullptr;
}

STextureObject * STextureManager::Load3DTexture( const char * filename )
{
	return nullptr;
}

SRenderableTexture* STextureManager::CreateRenderableTexture(const char* name, const SRenderableConfig& cfg)
{
	return new SRenderableTexture(name, cfg, *this);
}