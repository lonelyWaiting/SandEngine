#pragma once

#include "SandEngine/SApiDef.h"
#include "SandEngine/Resource/SResourceManager.h"

class STextureObject;

class STextureManager : public SResourceManager
{
public:
	SAND_API STextureObject* Load2DTexture( const char* filename );
	SAND_API STextureObject* Load3DTexture( const char* filename );
};