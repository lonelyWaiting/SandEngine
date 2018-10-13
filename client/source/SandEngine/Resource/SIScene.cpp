#include "SandEnginePCH.h"
#include "SIScene.h"
#include "SandEngine/Application/SandEngineModule.h"
#include "SandEngine/Resource/SStaticMeshManager.h"

SIMesh& SIScene::AddMesh( const char* name , eVertexAttribute vertexAttribute )
{
	m_Meshs.PushBack( SIMesh( name , vertexAttribute ) );
	return m_Meshs.Peek();
}

SIMaterial& SIScene::AddMaterial( const char* name )
{
	m_Materials.PushBack( SIMaterial( name ) );
	return m_Materials.Peek();
}

SIMaterial::SIMaterial( const char* name )
{
	m_name = name;
}

void SIMaterial::SetTexture( eTextureType mask , const char * tex )
{
	if( mask >= eTT_Max )	return;

	m_Textures[mask] = tex;
}

SIMesh::SIMesh( const char* name , eVertexAttribute vertexAttribute )
{
	m_name            = name;
	m_vertexAttribute = vertexAttribute;
}

SITriangle& SIMesh::AddTriangle()
{
	m_triangleList.PushBack( SITriangle() );
	return m_triangleList.Peek();
}

SIVertex& SIMesh::AddVertex()
{
	m_vertexList.PushBack( SIVertex() );
	return m_vertexList.Peek();
}

void SIMesh::SetVertexPos( int index , const SVector3f & pos )
{
	if( ( m_vertexAttribute & eVA_POSITION ) == eVA_POSITION && index >= 0 && index < ( int )m_vertexList.GetSize() )
	{
		m_vertexList[index].Pos = pos;
	}
}

void SIMesh::SetVertexNormal( int index , const SVector3f & normal )
{
	if( ( m_vertexAttribute & eVA_NORMAL ) == eVA_NORMAL && index >= 0 && index < ( int )m_vertexList.GetSize() )
	{
		m_vertexList[index].Normal = normal;
	}
}

void SIMesh::SetVertexTangent( int index , const SVector4f & tangent )
{
	if( ( m_vertexAttribute & eVA_TEXCOORD2 ) == eVA_TEXCOORD2 && index >= 0 && index < ( int )m_vertexList.GetSize() )
	{
		m_vertexList[index].Tangent = tangent;
	}
}

void SIMesh::SetVertexColor( int index , const SVector3f & color )
{
	if( ( m_vertexAttribute & eVA_VERTEXCOLOR ) == eVA_VERTEXCOLOR && index >= 0 && index < ( int )m_vertexList.GetSize() )
	{
		m_vertexList[index].vertexColor = color;
	}
}

void SIMesh::SetVertexUV( int index , const SVector2f & uv )
{
	if( ( m_vertexAttribute & eVA_TEXCOORD0 ) == eVA_TEXCOORD0 && index >= 0 && index < ( int )m_vertexList.GetSize() )
	{
		m_vertexList[index].UV = uv;
	}
}

void SIMesh::SetMaterialIndex( int polygonIndex , int materialIndex )
{
	if( polygonIndex >= 0 && polygonIndex < ( int )m_triangleList.GetSize() )
	{
		m_triangleList[polygonIndex].materialID = materialIndex;
	}
}

int SIMesh::GetNumOfTriangle() const
{
	return m_triangleList.GetSize();
}

int SIMesh::GetNumOfVertices() const
{
	return m_vertexList.GetSize();
}

const SIVertex& SIMesh::GetVertex( int index ) const
{
	assert( index >= 0 && index < ( int )m_vertexList.GetSize() );
	return m_vertexList[index];
}

const SITriangle& SIMesh::GetTriangle( int index ) const
{
	assert( index >= 0 && index < ( int )m_triangleList.GetSize() );
	return m_triangleList[index];
}

eVertexAttribute SIMesh::GetVertexAttribute()
{
	return m_vertexAttribute;
}

const char* SIMesh::GetName()
{
	return m_name.AsChar();
}

#include "SStaticMesh.h"

SStaticMesh* SIScene::Process()
{
	SArray<int> triangleList;
	int iVertexNum = 0 , iTriangleNum = 0;
	for( int i = 0 , size = m_Meshs.GetSize(); i < size; i++ )
	{
		iVertexNum   += m_Meshs[i].GetNumOfVertices();
		iTriangleNum += m_Meshs[i].GetNumOfTriangle();
	}
	triangleList.Reserve( iTriangleNum * 3 );

	SArray<int>	materialList;
	for( int i = 0 , size = m_Meshs.GetSize(); i < size; i++ )
	{
		materialList.Reserve( size );
		materialList.PushBack( -1 );

		const SIMesh& mesh = m_Meshs[i];
		for( int j = 0 , NumOfTriangle = mesh.GetNumOfTriangle(); j < NumOfTriangle; j++ )
		{
			const SITriangle& tri = mesh.GetTriangle( j );
			triangleList.PushBack( tri.vertexID[0] );
			triangleList.PushBack( tri.vertexID[1] );
			triangleList.PushBack( tri.vertexID[2] );
			if( materialList[i] == -1 )	materialList[i] = tri.materialID;
		}
	}

	SStaticMesh* pMesh = SandEngine::StaticMeshManager.LoadStaticMesh(m_Filename.AsChar());
	SMeshBuffer* pMeshBuffer = new SMeshBuffer();

	eVertexAttribute va = m_Meshs[0].GetVertexAttribute();

	int offset = 0;
	SVertexDescription desc;
	if( va & eVA_POSITION )
	{
		desc.m_iPos = (eVF_Float3 << 24) | offset;
		offset += sizeof( float ) * 3;
	}

	if( va & eVA_NORMAL )
	{
		desc.m_iNormal = (eVF_Float3 << 24) | offset;
		offset += sizeof( float ) * 3;
	}

	if( va & eVA_TEXCOORD2 )
	{
		desc.m_iTexcoord[2] = (eVF_Float4 << 24) | offset;
		offset += sizeof( float ) * 4;
	}

	if( va & eVA_TEXCOORD0 )
	{
		desc.m_iTexcoord[0] = (eVF_Float2 << 24) | offset;
		offset += sizeof( float ) * 2;
	}

	if( va & eVA_VERTEXCOLOR )
	{
		desc.m_color = (eVF_Float3 << 24) | offset;
		offset += sizeof( float ) * 3;
	}
	desc.stride = offset;

	char* pVertex = new char[iVertexNum * offset];
	for( int i = 0 , submeshCount = m_Meshs.GetSize(); i < submeshCount; i++ )
	{
		SIMesh& mesh = m_Meshs[i];
		for( int ivertex = 0 , vertexCount = mesh.GetNumOfVertices(); ivertex < vertexCount; ivertex++ )
		{
			const SIVertex& vert = mesh.m_vertexList[ivertex];
			if( va & eVA_POSITION )
			{
				memcpy( pVertex , &vert.Pos , sizeof( float ) * 3 );
				pVertex += sizeof( float ) * 3;
			}

			if( va & eVA_NORMAL )
			{
				memcpy( pVertex , &vert.Normal , sizeof( float ) * 3 );
				pVertex += sizeof( float ) * 3;
			}

			if( va & eVA_TEXCOORD2 )
			{
				memcpy( pVertex , &vert.Tangent , sizeof( float ) * 4 );
				pVertex += sizeof( float ) * 4;
			}

			if( va & eVA_TEXCOORD0 )
			{
				memcpy( pVertex , &vert.UV , sizeof( float ) * 2 );
				pVertex += sizeof( float ) * 2;
			}

			if( va & eVA_VERTEXCOLOR )
			{
				memcpy( pVertex , &vert.vertexColor , sizeof( float ) * 3 );
				pVertex += sizeof( float ) * 3;
			}
		}
	}

	pMeshBuffer->EnsureVertexBuffer( desc , eMemUsage( eBU_Dynamic | eBU_UAV_ByteAddress ) , iVertexNum , pVertex , eBF_SRV );
	pMeshBuffer->EnsureIndexBuffer( ( eMemUsage )( eBU_Dynamic | eBU_UAV_ByteAddress ) , iTriangleNum * 3 , triangleList.GetData() , eIF_Int , eBF_SRV );
	SAFE_DELETE_ARRAY( pVertex );

	pMesh->SetMeshBuffer( pMeshBuffer );

	for( int i = 0 , size = m_Materials.GetSize(); i < size; i++ )
	{
		SIMaterial& mat = m_Materials[i];
		pMesh->m_matList.PushBack( SMatObj( mat.m_name.AsChar() ) );
		pMesh->m_matList[i];
	}

	for( int i = 0 , submeshCount = m_Meshs.GetSize() , vertexOffset = 0 , indexOffset = 0; i < submeshCount; i++ )
	{
		SIMesh& mesh = m_Meshs[i];
		pMesh->m_subMeshes.PushBack( SBaseSubmesh( mesh.GetName() ) );
		SBaseSubmesh& submesh = pMesh->m_subMeshes.Peek();
		submesh.iVertexStart  = vertexOffset;
		submesh.iVertexNum    = mesh.GetNumOfVertices();
		submesh.iIndexStart   = indexOffset;
		submesh.iIndexNum     = mesh.GetNumOfTriangle() * 3;
		submesh.matID         = materialList[i];
		vertexOffset += submesh.iVertexNum;
		indexOffset  += submesh.iIndexNum;
	}
	
	return pMesh;
}

void SIScene::SetFilename( const char* filename )
{
	this->m_Filename = filename;
}