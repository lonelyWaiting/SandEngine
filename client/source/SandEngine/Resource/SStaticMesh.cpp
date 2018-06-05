#include "SandEnginePCH.h"
#include "SStaticMesh.h"
#include "SResourceManager.h"

SStaticMesh::SStaticMesh( const char * filename , SResourceManager& manager )
	:SResource( filename , manager )
{

}

int SStaticMesh::GetSubmeshCount()
{
	return m_subMeshes.GetSize();
}

const SBaseSubmesh& SStaticMesh::GetSubmesh( int i ) const
{
	assert( i >= 0 && i < ( int )m_subMeshes.GetSize() );
	return m_subMeshes[i];
}

void SStaticMesh::SetMeshBuffer( SMeshBuffer* pMeshBuffer /* = nullptr */ )
{
	m_MeshBuffer = pMeshBuffer;
}

void SStaticMesh::Ensureloaded()
{

}

void SStaticMesh::EnsureUnloaded()
{

}

#include "FbxImporter.h"
#include "SIScene.h"

SStaticMesh* LoadMesh( const char* filename )
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