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
	if( ( m_vertexAttribute & eVA_POSITION ) == eVA_POSITION && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].Pos = pos;
	}
}

void SIMesh::SetVertexNormal( int index , const SVector3f & normal )
{
	if( ( m_vertexAttribute & eVA_NORMAL ) == eVA_NORMAL && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].Normal = normal;
	}
}

void SIMesh::SetVertexTangent( int index , const SVector4f & tangent )
{
	if( ( m_vertexAttribute & eVA_TANGENT ) == eVA_TANGENT && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].Tangent = tangent;
	}
}

void SIMesh::SetVertexBinormal( int index , const SVector3f & biNormal )
{
	if( ( m_vertexAttribute & eVA_BINORMAL ) == eVA_BINORMAL && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].Binormal = biNormal;
	}
}

void SIMesh::SetVertexColor( int index , const SVector3f & color )
{
	if( ( m_vertexAttribute & eVA_VERTEXCOLOR ) == eVA_VERTEXCOLOR && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].vertexColor = color;
	}
}

void SIMesh::SetVertexUV( int index , const SVector2f & uv )
{
	if( ( m_vertexAttribute & eVA_UV ) == eVA_UV && index >= 0 && index < ( int )m_Vertices.GetSize() )
	{
		m_Vertices[index].UV = uv;
	}
}

void SIMesh::SetMaterialIndex( int polygonIndex , int materialIndex )
{
	if( polygonIndex >= 0 && polygonIndex < ( int )m_Triangles.GetSize() )
	{
		m_Triangles[polygonIndex].materialID = materialIndex;
	}
}

int SIMesh::GetNumOfTriangle() const
{
	return m_Triangles.GetSize();
}

int SIMesh::GetNumOfVertices() const
{
	return m_Vertices.GetSize();
}

const SIVertex& SIMesh::GetVertex( int index ) const
{
	assert( index >= 0 && index < ( int )m_Vertices.GetSize() );
	return m_Vertices[index];
}

const SITriangle& SIMesh::GetTriangle( int index ) const
{
	assert( index >= 0 && index < ( int )m_Triangles.GetSize() );
	return m_Triangles[index];
}

eVertexAttribute SIMesh::GetVertexAttribute()
{
	return m_vertexAttribute;
}

#include "SMesh.h"

SMesh* SIScene::Process()
{
	SArray<SIVertex> vertexList;
	SArray<int> triangleList;
	int iVertexNum = 0 , iTriangleNum = 0;
	for( int i = 0 , size = m_Meshs.GetSize(); i < size; i++ )
	{
		iVertexNum += m_Meshs[i].GetNumOfVertices();
		iTriangleNum += m_Meshs[i].GetNumOfTriangle();
	}
	vertexList.Reserve( iVertexNum );
	triangleList.Reserve( iTriangleNum * 3 );

	for( int i = 0 , size = m_Meshs.GetSize(); i < size; i++ )
	{
		const SIMesh& mesh = m_Meshs[i];
		for( int j = 0 , numOfVertices = mesh.GetNumOfVertices(); j < numOfVertices; j++ )
		{
			vertexList.PushBack( mesh.GetVertex( i ) );
		}

		for( int j = 0 , NumOfTriangle = mesh.GetNumOfTriangle(); j < NumOfTriangle; j++ )
		{
			const SITriangle& tri = mesh.GetTriangle( j );
			triangleList.PushBack( tri.vertexID[0] );
			triangleList.PushBack( tri.vertexID[1] );
			triangleList.PushBack( tri.vertexID[2] );
		}
	}

	SMesh* pMesh = nullptr;
	if( iVertexNum > 0 )
	{
		pMesh = new SMesh( m_Filename.AsChar() );
		SMeshBuffer* pMeshBuffer = new SMeshBuffer();

		eVertexAttribute va = m_Meshs[0].GetVertexAttribute();

		int offset = 0;
		SVertexDescription desc;
		if( va & eVA_POSITION )
		{
			desc.m_iPos = eVF_Float3 | offset;
			offset += sizeof( float ) * 3;
		}

		if( va & eVA_NORMAL )
		{
			desc.m_iNormal = eVF_Float3 | offset;
			offset += sizeof( float ) * 3;
		}

		if( va & eVA_TANGENT )
		{
			desc.m_iTangent = eVF_Float4 | offset;
			offset += sizeof( float ) * 4;
		}

		if( va & eVA_BINORMAL )
		{
			desc.m_iBinormal = eVF_Float3 | offset;
			offset += sizeof( float ) * 3;
		}

		if( va & eVA_UV )
		{
			desc.m_iTexcoord[0] = eVF_Float2 | offset;
			offset += sizeof( float ) * 2;
		}

		if( va & eVA_VERTEXCOLOR )
		{
			desc.m_color = eVF_Float3 | offset;
			offset += sizeof( float ) * 3;
		}
		desc.stride = offset;

		pMeshBuffer->EnsureVertexBuffer( desc , eBU_Dynamic , iVertexNum , vertexList.GetData() , eBBF_SRV );
		pMeshBuffer->EnsureIndexBuffer( eBU_Dynamic , iTriangleNum * 3 , triangleList.GetData() , eIF_Short , eBBF_SRV );

		pMesh->SetMeshBuffer( pMeshBuffer );

		// TODO: Multi Submesh
		SMaterial mat;
		mat.iNumOfVertices = iVertexNum;
		mat.iNumOfTriangle = iTriangleNum;
		mat.iVertexOffset  = 0;
		mat.iIndexOffset   = 0;
		pMesh->m_Materials.PushBack( mat );
	}
	
	return pMesh;
}

void SIScene::SetFilename( const char* filename )
{
	this->m_Filename = filename;
}