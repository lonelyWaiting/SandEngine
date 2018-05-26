#pragma once

#include "SandBase/Vector/SArray.h"
#include "SMeshBuffer.h"
#include "SandEngine/Application/SandEngineModule.h"
#include "SandEngine/Model/Material/Material.h"

class SBaseSubmesh
{
public:
	SBaseSubmesh( const char* );

private:
	SString	name;
	int iVertexStart = 0;
	int iVertexNum   = 0;
	int iIndexStart  = 0;
	int iIndexNum    = 0;
	int matID        = -1;

	friend class SIScene;
};

class SMesh : public SResource
{
public:
	SMesh( const char* filename );
	void SetMeshBuffer( SMeshBuffer* pMeshBuffer = nullptr );
	int GetSubmeshCount();
	const SBaseSubmesh& GetSubmesh( int i ) const;

private:
	SArray<SBaseSubmesh> m_subMeshes;
	SMeshBufferPtr       m_MeshBuffer;
	SArray<SMatObj>		 m_matList;
	friend class SIScene;
};

SAND_API SMesh* LoadMesh( const char* filename );