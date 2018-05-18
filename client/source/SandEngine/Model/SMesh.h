#pragma once

#include "SandBase/Vector/SArray.h"
#include "SMeshBuffer.h"
#include "SandEngine/Application/SandEngineModule.h"

struct SMaterial
{
	int iNumOfVertices = 0;
	int iNumOfTriangle = 0;
	int iVertexOffset  = 0;
	int iIndexOffset   = 0;

	// texture object
};

class SMesh : public SResource
{
public:
	SMesh( const char* filename );
	int GetMaterialCount();
	void SetMeshBuffer( SMeshBuffer* pMeshBuffer = nullptr );
	
private:
	SArray<SMaterial> m_Materials;
	SMeshBufferPtr m_MeshBuffer;
	friend class SIScene;
};

SAND_API SMesh* LoadMesh( const char* filename );