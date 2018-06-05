#pragma once

#include "SandEngine/SApiDef.h"
#include "SandBase/Object/SSmartPointer.h"
#include "SandEngine/Resource/SResourceManager.h"

enum eShaderStage
{
	eST_Vertex ,
	eST_Pixel ,
	eST_Geometry ,
	eST_Hull,
	eST_Domain,
	eST_Compute,
	eST_Max
};

class SShader
{
public:
	SShader() { m_shaderbytecode = nullptr; }
	virtual void CreateShader( ID3D11Device* device ) = 0;

public:
	eShaderStage m_shaderType;
	ID3DBlob* m_shaderbytecode;
};

typedef SSmartPointer<struct ID3D11VertexShader> D3DVertexShaderPtr;

class SVertexShader : public SShader
{
public:
	SVertexShader();
	ID3D11VertexShader* GetVertexShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DVertexShaderPtr m_vertexShader;
};

typedef SSmartPointer<struct ID3D11PixelShader> D3DPixelShader;

class SPixelShader : public SShader
{
public:
	SPixelShader();
	ID3D11PixelShader* GetPixelShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DPixelShader m_pixelShader;
};

typedef SSmartPointer<struct ID3D11GeometryShader> D3DGeometryShaderPtr;

class SGeometryShader : public SShader
{
public:
	SGeometryShader();
	ID3D11GeometryShader* GetGeometryShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DGeometryShaderPtr m_geometryShader;
};

typedef SSmartPointer<struct ID3D11HullShader>		D3DHullShaderPtr;

class SHullShader : public SShader
{
public:
	SHullShader();
	ID3D11HullShader* GetHullShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DHullShaderPtr m_hullShader;
};

typedef SSmartPointer<struct ID3D11DomainShader>	D3DDomainShaderPtr;

class SDomainShader : public SShader
{
public:
	SDomainShader();
	ID3D11DomainShader* GetDomainShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DDomainShaderPtr m_domainShader;
};

typedef SSmartPointer<struct ID3D11ComputeShader>	D3DComputeShaderPtr;

class SComputeShader : public SShader
{
public:
	SComputeShader();
	ID3D11ComputeShader* GetComputeShader();
	virtual void CreateShader( ID3D11Device* device );

private:
	D3DComputeShaderPtr m_computeShader;
};

class SShaderResource : public SResource
{
public:
	SShaderResource( const char* filename , SResourceManager& manager );

protected:
	SVertexShader       m_vertexShader;
	SPixelShader        m_pixelShader;
	SGeometryShader     m_geometryShader;
	SHullShader			m_hullShader;
	SDomainShader		m_domainShader;
	SComputeShader      m_computeShader;
	suInt32				m_inputShaderMask;
};

class ShaderManager : public SResourceManager
{
public:
	SAND_API SShaderResource* LoadShader( const char* filename );
};