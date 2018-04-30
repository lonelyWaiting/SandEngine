#include "SandEnginePCH.h"
#include "SIndexBuffer.h"

SIndexBuffer::SIndexBuffer( eBufferUsage usage , int stride , const void * pInitData , eBufferBindFlag bindFlag /*= eBBF_None*/ , int miscFlag /*= 0*/ )
	:SBuffer( usage , eBT_Index , stride , pInitData , bindFlag , miscFlag )
{

}

void SIndexBuffer::SetIndexType( eIndexType type )
{
	m_IndexType = type;
}

eIndexType SIndexBuffer::GetIndexType()
{
	return m_IndexType;
}
