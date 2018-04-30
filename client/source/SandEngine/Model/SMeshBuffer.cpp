#include "SandEnginePCH.h"
#include "SMeshBuffer.h"

SMeshBuffer::SMeshBuffer()
{
	m_VertexBuffer = nullptr;
	m_IndexBuffer  = nullptr;
}

void SMeshBuffer::EnsureVertexBuffer( const SVertexDescription & desc , eBufferUsage usage , const void * pVertices , eBufferBindFlag bindFlag , int miscFlag )
{
	m_VertexBuffer = new SVertexBuffer( desc , usage , pVertices , bindFlag , miscFlag );
}

void SMeshBuffer::EnsureIndexBuffer( int stride , eBufferUsage usage , const short * pIndices , eBufferBindFlag bindFlag , int miscFlag )
{
	m_IndexBuffer = new SIndexBuffer( usage , stride , pIndices , bindFlag , miscFlag );
	m_IndexBuffer->SetIndexType( eIT_Short );
}

void SMeshBuffer::EnsureIndexBuffer( int stride , eBufferUsage usage , const int * pIndices , eBufferBindFlag bindFlag , int miscFlag )
{
	m_IndexBuffer = new SIndexBuffer( usage , stride , pIndices , bindFlag , miscFlag );
	m_IndexBuffer->SetIndexType( eIT_Int );
}

SVertexBuffer* SMeshBuffer::GetVertexBuffer()
{
	return m_VertexBuffer.GetPointer();
}

SIndexBuffer* SMeshBuffer::GetIndexBuffer()
{
	return m_IndexBuffer.GetPointer();
}