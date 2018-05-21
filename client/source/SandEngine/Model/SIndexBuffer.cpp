#include "SandEnginePCH.h"
#include "SIndexBuffer.h"

SIndexBuffer::SIndexBuffer( eMemUsage usage , int iNumOfIndices , eIndexFormat format , const void * pInitData , eBindFlag bindFlag /*= eBBF_None*/ )
	:SBuffer( usage , ( format == eIF_Short ? sizeof( short ) : sizeof( int ) ) , iNumOfIndices , pInitData , ( usage & eBU_StructureBuffer )? bindFlag : ( eBindFlag )( bindFlag | eBF_Index ) )
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