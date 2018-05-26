#pragma once

#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SVector4f.h"
#include "SandBase/Vector/SArray.h"
#include "SandBase/String/SString.h"

enum eVertexAttribute
{
	eVA_None        = 0      ,
	eVA_POSITION    = 1 << 0 ,
	eVA_UV          = 1 << 1 ,
	eVA_NORMAL      = 1 << 2 ,
	eVA_TANGENT     = 1 << 3 ,
	eVA_BINORMAL    = 1 << 4 ,
	eVA_VERTEXCOLOR = 1 << 5 ,
};

enum eTextureType
{
	eTT_Diffuse = 0 ,
	eTT_Normal      ,
	eTT_Specular    ,
	eTT_Roughness   ,
	eTT_Emissive    ,
	eTT_Anisotropy  ,
	eTT_Bump        ,
	eTT_Displacement,
	eTT_Max
};

struct SIVertex
{
	SVector3f Pos;
	SVector2f UV;
	SVector3f Normal;
	SVector4f Tangent;
	SVector3f Binormal;
	SVector4f vertexColor;
};

struct SITriangle
{
	int materialID = -1;
	int vertexID[3];
};

class SIMesh
{
public:
	SIMesh( const char* name , eVertexAttribute vertexAttribute );
	SITriangle &		AddTriangle();
	SIVertex&			AddVertex();
	void				SetVertexPos( int index , const SVector3f& pos );
	void				SetVertexNormal( int index , const SVector3f& normal );
	void				SetVertexTangent( int index , const SVector4f& tangent );
	void				SetVertexBinormal( int index , const SVector3f& biNormal );
	void				SetVertexColor( int index , const SVector3f& color );
	void				SetVertexUV( int index , const SVector2f& uv );
	void				SetMaterialIndex( int polygonIndex , int materialIndex );
	int					GetNumOfVertices() const;
	int					GetNumOfTriangle() const;
	const SIVertex&		GetVertex( int index ) const;
	const SITriangle&	GetTriangle( int index ) const;
	eVertexAttribute	GetVertexAttribute();
	const char*			GetName();

public:
	SString				m_name;
	eVertexAttribute	m_vertexAttribute;
	SArray<SIVertex>    m_vertexList;
	SArray<SITriangle>	m_triangleList;
};

class SIMaterial
{
public:
	SIMaterial( const char* name );
	void SetTexture( eTextureType mask , const char* tex );

public:
	SString m_Textures[eTT_Max];
	SString m_name;
};

class SIScene
{
public:
	SIMesh & AddMesh( const char* name , eVertexAttribute vertexAttribute );
	SIMaterial& AddMaterial( const char* name );
	void SetFilename( const char* filename );
	class SMesh* Process();

protected:
	SArray<SIMesh>		m_Meshs;
	SArray<SIMaterial>	m_Materials;
	SString				m_Filename;
};