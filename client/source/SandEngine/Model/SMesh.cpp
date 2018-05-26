#include "SandEnginePCH.h"
#include "SMesh.h"

SMesh::SMesh( const char * filename )
	:SResource( filename )
{

}

int SMesh::GetSubmeshCount()
{
	return m_subMeshes.GetSize();
}

const SBaseSubmesh& SMesh::GetSubmesh( int i ) const
{
	assert( i >= 0 && i < ( int )m_subMeshes.GetSize() );
	return m_subMeshes[i];
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

SBaseSubmesh::SBaseSubmesh( const char * name )
{
	this->name = name;
	iVertexStart = iVertexNum = 0;
	iIndexStart  = iIndexNum  = 0;
	matID = -1;
}
