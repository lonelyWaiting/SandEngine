#include "SandEnginePCH.h"
#include "SVertexBuffer.h"

#define eVM_None		0
#define eVM_Position	SBIT(0)
#define eVM_Normal		SBIT(1)
#define eVM_Tangent		SBIT(2)
#define eVM_Binormal	SBIT(3)
#define eVM_Texcoord0	SBIT(4)
#define eVM_Texcoord1	SBIT(5)
#define eVM_Texcoord2	SBIT(6)
#define eVM_Texcoord3	SBIT(7)
#define eVM_Texcoord4	SBIT(8)
#define eVM_Texcoord5	SBIT(9)
#define eVM_Texcoord6	SBIT(10)
#define eVM_Texcoord7	SBIT(11)
#define eVM_Texcoord8	SBIT(12)
#define eVM_Texcoord9	SBIT(13)
#define eVM_Texcoord10	SBIT(14)
#define eVM_Texcoord11	SBIT(15)
#define eVM_Texcoord12	SBIT(16)
#define eVM_Texcoord13	SBIT(17)
#define eVM_Texcoord14	SBIT(18)
#define eVM_Texcoord15	SBIT(19)
#define eVM_Color		SBIT(20)

SVertexBuffer::SVertexBuffer( const SVertexDescription & desc , eBufferUsage usage , const void * pInitData /*= nullptr*/ , eBufferBindFlag viewFlag /*= eBBF_None*/ , int miscFlag /*= 0*/ )
	:SBuffer( usage , eBT_Vertex , desc.stride , pInitData , viewFlag , miscFlag )
{
	m_vertexDesc = desc;
}


int SVertexBuffer::GetVertexMask()
{
	int result = eVM_None;

	if( m_vertexDesc.m_iPos != -1 )
	{
		result |= eVM_Position;
	}

	if( m_vertexDesc.m_iNormal != -1 )
	{
		result |= eVM_Normal;
	}

	if( m_vertexDesc.m_iTangent != -1 )
	{
		result |= eVM_Tangent;
	}

	if( m_vertexDesc.m_iBinormal != -1 )
	{
		result |= eVM_Binormal;
	}

	for( int i = 0; i < 16; i++ )
	{
		if( m_vertexDesc.m_iTexcoord[i] != -1 )
		{
			result |= ( eVM_Texcoord0 << i );
		}
	}

	if( m_vertexDesc.m_color != -1 )
	{
		result |= eVM_Color;
	}

	return result;
}