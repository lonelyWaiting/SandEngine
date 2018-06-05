#pragma once

#include "SandEngine/SApiDef.h"
#include "SResourceManager.h"

class SStaticMesh;

class SStaticMeshManager : public SResourceManager
{
public:
	SStaticMeshManager() {}
	SAND_API SStaticMesh* LoadStaticMesh( const char* filename );
};