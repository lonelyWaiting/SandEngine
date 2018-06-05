#include "SandEnginePCH.h"
#include "SStaticMeshManager.h"
#include "SStaticMesh.h"

SStaticMesh* SStaticMeshManager::LoadStaticMesh( const char * filename )
{
	SStaticMesh* mesh = ( SStaticMesh* )SStaticMeshManager::FindResourceByName( filename );
	if( !mesh )
	{
		mesh = new SStaticMesh( filename , *this );
	}

	return mesh;
}