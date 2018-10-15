#include "SandEnginePCH.h"
#include "SMeshBuffer.h"
#include "SandEngine/Application/SRenderer.h"

void SMeshBuffer::EnsureVertexBuffer( const SVertexDescription & desc , eMemUsage usage , int iNumOfVertices , const void * pVertices , eBindFlag bindFlag )
{
	m_VertexBuffer = new SVertexBuffer( desc , usage , iNumOfVertices , pVertices , bindFlag );
}

void SMeshBuffer::EnsureIndexBuffer( eMemUsage usage , int iNumOfIndices , const void* pIndices , eIndexFormat format, eBindFlag bindFlag )
{
	m_IndexBuffer = new SIndexBuffer( usage , iNumOfIndices , format , pIndices , bindFlag );
}

const SVertexBuffer* SMeshBuffer::GetVertexBuffer() const
{
	return m_VertexBuffer.GetPointer();
}

const SIndexBuffer* SMeshBuffer::GetIndexBuffer() const
{
	return m_IndexBuffer.GetPointer();
}

int SMeshBuffer::GetNumOfVertices()
{
	return m_VertexBuffer ? m_VertexBuffer->GetNumOfVertices() : 0;
}

int SMeshBuffer::GetNumOfIndices()
{
	return m_IndexBuffer ? m_IndexBuffer->GetNumOfIndices() : 0;
}

void SMeshBuffer::SetFilename( const char* filename )
{
	if( m_VertexBuffer )	m_VertexBuffer->SetFilename( filename );
	if( m_IndexBuffer )		m_IndexBuffer->SetFilename( filename );
}