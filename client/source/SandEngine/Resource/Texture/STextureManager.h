#pragma once

#include "SandEngine/SApiDef.h"
#include "SandEngine/Resource/SResourceManager.h"

class STexture2D;
class SRenderableTexture;
struct SRenderableConfig;

class STextureManager : public SResourceManager
{
public:
	SAND_API STexture2D* Load2DTexture( const char* filename );
	SAND_API STexture2D* Load3DTexture( const char* filename );
};