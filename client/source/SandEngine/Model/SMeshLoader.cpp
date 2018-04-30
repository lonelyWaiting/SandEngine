#include "SandEnginePCH.h"
#include "SMeshLoader.h"
#include "SIScene.h"
#include "SMeshBuffer.h"

SMeshBuffer* SMeshLoader::LoadMeshBuffer( const char* filename )
{
	return nullptr;
}

SMeshBuffer * SMeshLoader::LoadFromScene( const SIScene& scene )
{
	SMeshBuffer* mesh = new SMeshBuffer();

	return mesh;
}
