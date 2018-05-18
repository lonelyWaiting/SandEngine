#include "SandEnginePCH.h"
#include "SMeshBuffer.h"
#include "SandEngine/Application/SRenderer.h"

void SMeshBuffer::EnsureVertexBuffer( const SVertexDescription & desc , eBufferUsage usage , int iNumOfVertices , const void * pVertices , eBufferBindFlag bindFlag , int miscFlag )
{
	m_VertexBuffer = new SVertexBuffer( desc , usage , iNumOfVertices , pVertices , bindFlag , miscFlag );
}

void SMeshBuffer::EnsureIndexBuffer( eBufferUsage usage , int iNumOfIndices , const void* pIndices , eIndexFormat format, eBufferBindFlag bindFlag , int miscFlag )
{
	m_IndexBuffer = new SIndexBuffer( usage , iNumOfIndices , format , pIndices , bindFlag , miscFlag );
}

SVertexBuffer* SMeshBuffer::GetVertexBuffer()
{
	return m_VertexBuffer.GetPointer();
}

SIndexBuffer* SMeshBuffer::GetIndexBuffer()
{
	return m_IndexBuffer.GetPointer();
}

int SMeshBuffer::GetNumOfVertices()
{
	return m_VertexBuffer.GetPointer() ? m_VertexBuffer->GetNumOfVertices() : 0;
}

int SMeshBuffer::GetNumOfIndices()
{
	return m_IndexBuffer.GetPointer() ? m_IndexBuffer->GetNumOfIndices() : 0;
}

void SMeshBuffer::SetFilename( const char* filename )
{
	if( m_VertexBuffer.GetPointer() != nullptr )	m_VertexBuffer->SetFilename( filename );
	if( m_IndexBuffer.GetPointer() != nullptr )		m_IndexBuffer->SetFilename( filename );
}