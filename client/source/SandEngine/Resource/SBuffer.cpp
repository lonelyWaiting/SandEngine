#include "SandEnginePCH.h"
#include "SBuffer.h"
#include "Application/SRenderer.h"
#include "SandBase/Log/SLog.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

// byte address size must be a multiple of four
DXGI_FORMAT GetSRVBufferFormat( eMemUsage usage )
{
	if( usage & eBU_StructureBuffer )
		return DXGI_FORMAT_UNKNOWN;
	else if( usage & eBU_UAV_ByteAddress )
		return DXGI_FORMAT_R32_TYPELESS;
	else
		return DXGI_FORMAT_R32_UINT;
}

DXGI_FORMAT GetUAVBufferFormat( eMemUsage usage )
{
	if( usage & eBU_UAV_ByteAddress )
		return DXGI_FORMAT_R32_TYPELESS;
	else 
		return DXGI_FORMAT_R32_UINT;
}

int GetUAVBufferFlag( eMemUsage usage )
{
	int flag = 0;
	if( usage & eBU_UAV_ByteAddress )
		flag = D3D11_BUFFER_UAV_FLAG_RAW;
	if( usage & eBU_UAV_Append )
		flag |= D3D11_BUFFER_UAV_FLAG_APPEND;
	if( usage & eBU_UAV_Count )
		flag |= D3D11_BUFFER_UAV_FLAG_COUNTER;

	return flag;
}

SBuffer::SBuffer( eMemUsage usage , int stride , int count , const void* pInitData /*= nullptr */, eBindFlag bindFlag /*= eBBF_None*/ )
{
	D3D11_BUFFER_DESC bufDesc = { 0 };
	if( usage & eBU_Dynamic )
	{
		bufDesc.Usage          = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if( usage & eBU_Static )
	{
		bufDesc.Usage          = D3D11_USAGE_IMMUTABLE;
		bufDesc.CPUAccessFlags = 0;
	}

	bufDesc.ByteWidth = stride * count;
	
	if( bindFlag & eBF_Vertex )         bufDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
	if( bindFlag & eBF_Index )	        bufDesc.BindFlags |= D3D11_BIND_INDEX_BUFFER;
	if( bindFlag & eBF_Constant )	    bufDesc.BindFlags |= D3D11_BIND_CONSTANT_BUFFER;
	if( bindFlag & eBF_SRV )		    bufDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if( bindFlag & eBF_UAV )		    bufDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	if( bindFlag & eBF_StreamOut )	    bufDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;

	if( usage & eBU_StructureBuffer )	bufDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	if( usage & eBU_UAV_ByteAddress )	bufDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476900(v=vs.85).aspx#Raw_Buffer_Views
	if( ( usage & eBU_UAV_ByteAddress ) && ( usage & eBU_StructureBuffer ) )
	{
		SLog::Error( "can't combine the D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS flag with D3D11_RESOURCE_MISC_BUFFER_STRUCTURED" );
		assert( false );
	}

	if( ( bindFlag & eBF_Constant ) && ( usage & eBU_UAV_ByteAddress ) )
	{
		SLog::Error( "can't specify D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS in MiscFlags if you specify D3D11_BIND_CONSTANT_BUFFER in BindFlags" );
		assert( false );
	}

	bufDesc.StructureByteStride = ( usage & eBU_StructureBuffer ) ? stride : 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem          = pInitData;
	data.SysMemPitch      = 0;
	data.SysMemSlicePitch = 0;
	
	SRenderHelper::g_Device->CreateBuffer( &bufDesc , pInitData ? &data : nullptr, &m_pBuffer );

	if( bindFlag & eBF_SRV )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory( &srvDesc , sizeof( srvDesc ) );
		srvDesc.Format                = GetSRVBufferFormat( usage );
		srvDesc.ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.NumElements  = ( usage & eBU_UAV_ByteAddress ) ? ( stride * count ) >> 2 : count;
		srvDesc.BufferEx.Flags        = usage & eBU_UAV_ByteAddress ? D3D11_BUFFEREX_SRV_FLAG_RAW : 0;
		SRenderHelper::g_Device->CreateShaderResourceView( m_pBuffer , &srvDesc , &m_pSRV );
	}

	if( bindFlag & eBF_UAV )
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		memset( &uavDesc , 0 , sizeof( uavDesc ) );
		uavDesc.Format              = GetUAVBufferFormat( usage );
		uavDesc.ViewDimension       = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements  = ( usage & eBU_UAV_ByteAddress ) ? ( stride * count ) >> 2 : count;
		uavDesc.Buffer.Flags        = GetUAVBufferFlag( usage );
		SRenderHelper::g_Device->CreateUnorderedAccessView( m_pBuffer , &uavDesc , &m_pUAV );
	}
}

SBuffer::~SBuffer()
{
	SAFE_RELEASE( m_pSRV );
	SAFE_RELEASE( m_pUAV );
	SAFE_RELEASE( m_pBuffer );
}

ID3D11ShaderResourceView * SBuffer::GetShaderResourceView() const
{
	return m_pSRV;
}

ID3D11UnorderedAccessView * SBuffer::GetUnorderedAccessView() const
{
	return m_pUAV;
}

ID3D11Buffer* SBuffer::GetBuffer() const
{
	return m_pBuffer;
}

void * SBuffer::Lock()
{
#ifdef _DEBUG
	if( !m_pBuffer )		SLog::Error( "buffer:%s is empty , can't lock" , "xxx" );
	else if( !m_Lockable )	SLog::Error( "buffer:%s can't be lock" , "xx" );
#endif

	void* result = nullptr;
	if( m_pBuffer && m_Lockable )
	{
		D3D11_MAPPED_SUBRESOURCE data;
		SRenderHelper::g_ImmediateContext->Map( m_pBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &data );
		result = ( void* )data.pData;
	}

	return result;
}

void SBuffer::UnLock()
{
	if( m_pBuffer && m_Lockable )
	{
		SRenderHelper::g_ImmediateContext->Unmap( m_pBuffer , 0 );
	}
}


void SBuffer::SetFilename( const char* filename )
{
	if( m_pBuffer )	m_pBuffer->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( filename ) - 1 , filename );
}

SVertexBuffer::SVertexBuffer( const SVertexDescription & desc , eMemUsage usage , int iNumOfVertices , const void * pInitData /*= nullptr*/ , eBindFlag bindFlag /*= eBBF_None*/ )
	:SBuffer( usage , desc.stride , iNumOfVertices , pInitData , ( usage & eBU_StructureBuffer ) ? bindFlag : ( eBindFlag )( eBF_Vertex | bindFlag ) )
{
	m_vertexDesc = desc;
	m_iNumOfVertices = iNumOfVertices;
}

int SVertexBuffer::GetVertexMask()
{
	int result = eVA_None;

	if( m_vertexDesc.m_iPos != -1 )
	{
		result |= eVA_POSITION;
	}

	if( m_vertexDesc.m_iNormal != -1 )
	{
		result |= eVA_NORMAL;
	}

	for( int i = 0; i < 16; i++ )
	{
		if( m_vertexDesc.m_iTexcoord[i] != -1 )
		{
			result |= ( eVA_TEXCOORD0 << i );
		}
	}

	if( m_vertexDesc.m_color != -1 )
	{
		result |= eVA_VERTEXCOLOR;
	}

	return result;
}

int SVertexBuffer::GetNumOfVertices()
{
	return m_iNumOfVertices;
}

SIndexBuffer::SIndexBuffer( eMemUsage usage , int iNumOfIndices , eIndexFormat format , const void * pInitData , eBindFlag bindFlag /*= eBBF_None*/ )
	:SBuffer( usage , ( format == eIF_Short ? sizeof( short ) : sizeof( unsigned int ) ) , iNumOfIndices , pInitData , ( usage & eBU_StructureBuffer ) ? bindFlag : ( eBindFlag )( bindFlag | eBF_Index ) )
{
	m_IndexType = format;
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