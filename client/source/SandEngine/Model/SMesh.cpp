#include "SandEnginePCH.h"
#include "SMesh.h"

SMesh::SMesh( const char * filename )
	:SResource( filename )
{

}

int SMesh::GetMaterialCount()
{
	return m_Materials.GetSize();
}

void SMesh::SetMeshBuffer( SMeshBuffer* pMeshBuffer /* = nullptr */ )
{
	m_MeshBuffer = pMeshBuffer;
}

#include "FbxImporter.h"
#include "SIScene.h"

SMesh* LoadMesh( const char* filename )
{
	SString file = filename;
	if( file.EndWith_NoCase( "fbx" ) )
	{
		SIScene scene;
		return fbx_importer( filename , scene ) ? scene.Process() : nullptr;
	}

	return nullptr;
}