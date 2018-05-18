#ifndef SVERTEX_BUFFER_H
#define SVERTEX_BUFFER_H

#include "SBuffer.h"
#include "SandBase/Object/SSmartPointer.h"

struct SVertexDescription
{
	int m_iPos          = -1;
	int m_iNormal       = -1;
	int m_iTangent      = -1;
	int m_iBinormal     = -1;
	int m_iTexcoord[16] = { -1 };
	int m_color         = -1;
	int stride          = 0;
};

class SVertexBuffer : public SBuffer
{
public:
	SVertexBuffer( const SVertexDescription& desc , eBufferUsage usage , int iNumOfVertices = 0, const void* pInitData = nullptr , eBufferBindFlag viewFlag = eBBF_None , int miscFlag = 0 );
	int GetVertexMask();
	int GetNumOfVertices();

private:
	SVertexDescription m_vertexDesc;
	int m_iNumOfVertices;
};

typedef SSmartPointer<SVertexBuffer> SVertexBufferPtr;
#endif