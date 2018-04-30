#pragma once

#include "SVertexBuffer.h"
#include "SIndexBuffer.h"
#include "SandBase/Object/SSmartPointer.h"

class SMeshBuffer
{
public:
	void EnsureVertexBuffer( const SVertexDescription& desc , eBufferUsage usage , const void* pVertices = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );
	void EnsureIndexBuffer( int stride , eBufferUsage usage , const short* pIndices = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );
	void EnsureIndexBuffer( int stride , eBufferUsage usage , const int* pIndices = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );

	SVertexBuffer* GetVertexBuffer();
	SIndexBuffer*  GetIndexBuffer();

private:
	SVertexBufferPtr m_VertexBuffer = nullptr;
	SIndexBufferPtr  m_IndexBuffer  = nullptr;
};