#include "SandEnginePCH.h"
#include "SIScene.h"

SIMesh& SIScene::AddMesh( const char* name , eVertexAttribute vertexAttribute )
{
	m_Meshs.PushBack( SIMesh( name , vertexAttribute ) );
	return m_Meshs.Peek();
}

SIMaterial& SIScene::AddMaterial()
{
	m_Materials.PushBack( SIMaterial() );
	return m_Materials.Peek();
}

void SIMaterial::SetTexture( eTextureType mask , const char * tex )
{
	if( mask >= eTT_Max )	return;

	m_Textures[mask] = tex;
}

SIMesh::SIMesh( const char* name , eVertexAttribute vertexAttribute )
{
	m_name = name;
	m_vertexAttribute = vertexAttribute;
}

SITriangle& SIMesh::AddTriangle()
{
	m_Triangles.PushBack( SITriangle() );
	return m_Triangles.Peek();
}

SIVertex& SIMesh::AddVertex()
{
	m_Vertices.PushBack( SIVertex() );
	return m_Vertices.Peek();
}

void SIMesh::SetVertexPos( int index , const SVector3f & pos )
{
	if( ( m_vertexAttribute & eVA_POSITION ) == eVA_POSITION && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].Pos = pos;
	}
}

void SIMesh::SetVertexNormal( int index , const SVector3f & normal )
{
	if( ( m_vertexAttribute & eVA_NORMAL ) == eVA_NORMAL && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].Normal = normal;
	}
}

void SIMesh::SetVertexTangent( int index , const SVector4f & tangent )
{
	if( ( m_vertexAttribute & eVA_TANGENT ) == eVA_TANGENT && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].Tangent = tangent;
	}
}

void SIMesh::SetVertexBinormal( int index , const SVector3f & biNormal )
{
	if( ( m_vertexAttribute & eVA_BINORMAL ) == eVA_BINORMAL && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].Binormal = biNormal;
	}
}

void SIMesh::SetVertexColor( int index , const SVector3f & color )
{
	if( ( m_vertexAttribute & eVA_VERTEXCOLOR ) == eVA_VERTEXCOLOR && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].vertexColor = color;
	}
}

void SIMesh::SetVertexUV( int index , const SVector2f & uv )
{
	if( ( m_vertexAttribute & eVA_UV ) == eVA_UV && index >= 0 && index < m_Vertices.GetSize() )
	{
		m_Vertices[index].UV = uv;
	}
}

void SIMesh::SetMaterialIndex( int polygonIndex , int materialIndex )
{
	if( polygonIndex >= 0 && polygonIndex < m_Triangles.GetSize() )
	{
		m_Triangles[polygonIndex].materialID = materialIndex;
	}
}
