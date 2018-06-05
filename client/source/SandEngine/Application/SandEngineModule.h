#pragma once

#include "SandEngine/SApiDef.h"

class SCallbackManager;
class SStaticMeshManager;
class STextureManager;

class SandEngine
{
public:
	SAND_API static SCallbackManager	Callback;
	SAND_API static SStaticMeshManager	StaticMeshManager;
	SAND_API static STextureManager		TextureManager;
};

void SandEngineInit();
void SandEngineDeInit();