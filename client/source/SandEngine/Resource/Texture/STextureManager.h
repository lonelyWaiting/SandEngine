#pragma once

#include "SandEngine/SApiDef.h"

class STexture2D;
namespace STextureManager
{
	SAND_API STexture2D* Load2DTextureFromFile(const char* filename);
};