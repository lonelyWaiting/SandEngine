#include "SandEnginePCH.h"
#include "SShader.h"
#include "SandBase/IO/SFileStream.h"
#include "SandBase/Log/SLog.h"
#include "SandEngine/Application/SRenderer.h"
#include "SandEngine/Resource/SBuffer.h"
#include "SandEngine/Pipeline/SRenderHelper.h"

SVertexShader::SVertexShader()
{
	m_shaderType   = eST_Vertex;
	m_vertexShader = nullptr;
}

ID3D11VertexShader * SVertexShader::GetVertexShader()
{
	return m_vertexShader;
}

void SVertexShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11VertexShader* pShader = nullptr;
	device->CreateVertexShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_vertexShader = pShader;
}

SPixelShader::SPixelShader()
{
	m_shaderType  = eST_Pixel;
	m_pixelShader = nullptr;
}

ID3D11PixelShader * SPixelShader::GetPixelShader()
{
	return m_pixelShader;
}

void SPixelShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11PixelShader* pShader = nullptr;
	device->CreatePixelShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_pixelShader = pShader;
}

SGeometryShader::SGeometryShader()
{
	m_shaderType     = eST_Geometry;
	m_geometryShader = nullptr;
}

ID3D11GeometryShader * SGeometryShader::GetGeometryShader()
{
	return m_geometryShader;
}

void SGeometryShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11GeometryShader* pShader = nullptr;
	device->CreateGeometryShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_geometryShader = pShader;
}

SHullShader::SHullShader()
{
	m_shaderType = eST_Hull;
	m_hullShader = nullptr;
}

ID3D11HullShader * SHullShader::GetHullShader()
{
	return m_hullShader;
}

void SHullShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11HullShader* pShader = nullptr;
	device->CreateHullShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_hullShader = pShader;
}

SDomainShader::SDomainShader()
{
	m_shaderType   = eST_Domain;
	m_domainShader = nullptr;
}

ID3D11DomainShader * SDomainShader::GetDomainShader()
{
	return m_domainShader;
}

void SDomainShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11DomainShader* pShader = nullptr;
	device->CreateDomainShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_domainShader = pShader;
}

SComputeShader::SComputeShader()
{
	m_shaderType    = eST_Compute;
	m_computeShader = nullptr;
}

ID3D11ComputeShader * SComputeShader::GetComputeShader()
{
	return m_computeShader;
}

void SComputeShader::CreateShader( ID3D11Device * device )
{
	if( !m_shaderbytecode || !device )	return;

	ID3D11ComputeShader* pShader = nullptr;
	device->CreateComputeShader( m_shaderbytecode->GetBufferPointer() , m_shaderbytecode->GetBufferSize() , nullptr , &pShader );
	m_computeShader = pShader;
}

SShaderResource* ShaderManager::LoadShader( const char * filename )
{
	SShaderResource* shader = ( SShaderResource* )FindResourceByName( filename );
	if( shader )	return shader;

	shader = new SShaderResource( filename , *this );
	return shader;
}

SShaderResource::SShaderResource( const char * filename , SResourceManager & manager )
	:SResource( filename , manager )
{
	m_inputShaderMask = eVA_None;

	suInt32 shaderCompileFlag = 0;
#ifdef _DEBUG
	shaderCompileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

	ID3DBlob* errorMessage = nullptr;

	SFileInStream fi;
	if( fi.OpenFile( filename ) )
	{
		int size = fi.GetFileSize();
		char* data = new char[size];
		fi.Read( data , size );
		fi.Close();

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"vs_main" ,
								"vs_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_vertexShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile vertex shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"ps_main" ,
								"ps_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_pixelShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile pixel shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"gs_main" ,
								"gs_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_geometryShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile geometry shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"hs_main" ,
								"hs_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_hullShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile hull shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"ds_main" ,
								"ds_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_domainShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile domain shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}

		if( FAILED( D3DCompile( data ,
								size ,
								nullptr ,
								nullptr ,
								D3D_COMPILE_STANDARD_FILE_INCLUDE ,
								"cs_main" ,
								"cs_5_0" ,
								shaderCompileFlag ,
								0 ,
								&m_computeShader.m_shaderbytecode ,
								&errorMessage ) ) )
		{
			SLog::Error( "compile compute shader failed, filepath:%s" , filename );
			SLog::Error( "errorMessage:%s" , errorMessage ? ( const char* )errorMessage->GetBufferPointer() : "" );
		}
	}

	SRenderHelper::CreateShader( m_vertexShader );
	SRenderHelper::CreateShader( m_pixelShader );
	SRenderHelper::CreateShader( m_geometryShader );
	SRenderHelper::CreateShader( m_hullShader );
	SRenderHelper::CreateShader( m_domainShader );
	SRenderHelper::CreateShader( m_computeShader );

	// reflect vertex shader input mask
	ID3D11ShaderReflection* reflector = nullptr;
	if( ID3DBlob* vertexBlob = m_vertexShader.m_shaderbytecode )
	{
		if( FAILED( D3DReflect( vertexBlob->GetBufferPointer() ,
								vertexBlob->GetBufferSize() ,
								IID_ID3D11ShaderReflection ,
								reinterpret_cast< void** >( &reflector ) ) ) )
		{
			SLog::Error( "failed to create shader reflection interface!" );
			return;
		}

		D3D11_SHADER_DESC shaderDesc;
		reflector->GetDesc( &shaderDesc );

		suInt32 shaderMask = eVA_None;

		for (int i = 0; i < (int)shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
			reflector->GetInputParameterDesc( i , &inputDesc );
			SString semanticName = inputDesc.SemanticName;
			if( semanticName.IsEqual( "POSITION" ) )
			{
				shaderMask |= eVA_POSITION;
			}
			else if( semanticName.IsEqual( "NORMAL" ) )
			{
				shaderMask |= eVA_NORMAL;
			}
			else if( semanticName.IsEqual( "VERTEXCOLOR" ) )
			{
				shaderMask |= eVA_VERTEXCOLOR;
			}
			else if( semanticName.IsEqual( "TEXCOORD" ) )
			{
				shaderMask |= ( eVA_TEXCOORD0 << inputDesc.SemanticIndex );
			}
		}
	}

	auto reflect_constant_buffer = []( ID3DBlob* shaderbytecode )
	{
		if( !shaderbytecode )	return;

		ID3D11ShaderReflection* reflector = nullptr;
		if( FAILED( D3DReflect( shaderbytecode->GetBufferPointer() ,
								shaderbytecode->GetBufferSize() ,
								IID_ID3D11ShaderReflection ,
								reinterpret_cast< void** >( &reflector ) ) ) )
		{
			SLog::Error( "failed to create shader reflection interface!" );
			return;
		}

		D3D11_SHADER_DESC shaderDesc;
		reflector->GetDesc( &shaderDesc );

		for (int i = 0; i < (int)shaderDesc.ConstantBuffers; i++)
		{
			ID3D11ShaderReflectionConstantBuffer* pConstantBuffer = reflector->GetConstantBufferByIndex( i );

			D3D11_SHADER_BUFFER_DESC bufferDesc;
			pConstantBuffer->GetDesc( &bufferDesc );

			if( bufferDesc.Type == D3D_CT_CBUFFER || bufferDesc.Type == D3D_CT_TBUFFER )
			{

			}
		}
	};
}