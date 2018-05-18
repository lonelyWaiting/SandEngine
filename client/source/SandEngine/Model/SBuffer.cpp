#include "SandEnginePCH.h"
#include "SBuffer.h"
#include "Application/SRenderer.h"
#include "SandBase/Log/SLog.h"

SBuffer::SBuffer( eBufferUsage usage , eBufferType type , int stride , const void* pInitData /*= nullptr */, eBufferBindFlag bindFlag /*= eBBF_None*/, int miscFlag /*= 0*/ )
{
	if( bindFlag & eBBF_UAV )	usage = eBU_Default;

	D3D11_BUFFER_DESC bufDesc;
	switch( usage )
	{
		case eBU_Default:
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			break;
		case eBU_Static:
			bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
			break;
		case eBU_Dynamic:
			bufDesc.Usage          = D3D11_USAGE_DYNAMIC;
			bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_Lockable = true;
			break;
		case eBU_Staging:
			bufDesc.Usage = D3D11_USAGE_STAGING;
			break;
	}
	bufDesc.ByteWidth = stride;
	
	switch( type )
	{
		case eBT_Vertex:
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			break;
		case eBT_Index:
			bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;
		case eBT_Constant:
			bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			break;
	}

	if( bindFlag & eBBF_SRV )	        bufDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if( bindFlag & eBBF_UAV )	        bufDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	if( bindFlag & eBBF_StreamOut )	    bufDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;

	bufDesc.MiscFlags           = miscFlag;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem          = pInitData;
	data.SysMemPitch      = 0;
	data.SysMemSlicePitch = 0;

	SRenderer::Get().GetDevice()->CreateBuffer( &bufDesc , &data , &m_pBuffer );

	if( bindFlag & eBBF_SRV )
	{
		SRenderer::Get().GetDevice()->CreateShaderResourceView( m_pBuffer , nullptr , &m_pSRV );
	}

	if( bindFlag & eBBF_UAV )
	{
		SRenderer::Get().GetDevice()->CreateUnorderedAccessView( m_pBuffer , nullptr , &m_pUAV );
	}
}

SBuffer::~SBuffer()
{
	if( m_pSRV )	SAFE_RELEASE( m_pSRV );
	if( m_pUAV )	SAFE_RELEASE( m_pUAV );
	if( m_pBuffer )	SAFE_RELEASE( m_pBuffer );
}

ID3D11ShaderResourceView * SBuffer::GetShaderResourceView() const
{
	return m_pSRV;
}

ID3D11UnorderedAccessView * SBuffer::GetUnorderedAccessView() const
{
	return m_pUAV;
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
		SRenderer::Get().GetDeviceContext()->Map( m_pBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &data );
		result = ( void* )data.pData;
	}

	return result;
}

void SBuffer::UnLock()
{
	if( m_pBuffer && m_Lockable )
	{
		SRenderer::Get().GetDeviceContext()->Unmap( m_pBuffer , 0 );
	}
}


void SBuffer::SetFilename( const char* filename )
{
	if( m_pBuffer )	m_pBuffer->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( filename ) - 1 , filename );
}