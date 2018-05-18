#include "SandEnginePCH.h"
#include "SIndexBuffer.h"

SIndexBuffer::SIndexBuffer( eBufferUsage usage , int iNumOfIndices , eIndexFormat format , const void * pInitData , eBufferBindFlag bindFlag /*= eBBF_None*/ , int miscFlag /*= 0*/ )
	:SBuffer( usage , eBT_Index , iNumOfIndices *  ( format == eIF_Short ? sizeof( short ) : sizeof( int ) ) , pInitData , bindFlag , miscFlag )
{
	m_IndexType     = format;
	m_iNumOfIndices = iNumOfIndices;
}

eIndexFormat SIndexBuffer::GetIndexType()
{
	return m_IndexType;
}

int SIndexBuffer::GetNumOfIndices()
{
	return m_iNumOfIndices;
}