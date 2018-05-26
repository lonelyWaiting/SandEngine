#include "SandEnginePCH.h"
#include "FbxImporter.h"
#include <fbxsdk.h>
#include "Model/SIScene.h"
#include "SandBase/Log/SLog.h"

void import_mesh_node( FbxNode* pNode , SArray<FbxNode*>& meshNodeList )
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if( pNodeAttribute && pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh )
	{
		meshNodeList.PushBack( pNode );
	}

	for( int i = 0 , iChildCount = pNode->GetChildCount(); i < iChildCount; i++ )
	{
		import_mesh_node( pNode->GetChild( i ) , meshNodeList );
	}
}

bool triangluate( FbxMesh*& pMesh , FbxGeometryConverter& converter )
{
	if( !pMesh )	return false;
	if( pMesh->IsTriangleMesh() )	return true;

	pMesh = ( FbxMesh* )converter.Triangulate( pMesh , true );
	
	return pMesh->IsTriangleMesh();
}

eVertexAttribute import_vertex_attribute( FbxMesh* pMesh )
{
	eVertexAttribute vertexAttribute = eVA_POSITION;

	vertexAttribute = pMesh->GetElementNormalCount()      > 0 ? (eVertexAttribute)( eVA_NORMAL  | vertexAttribute ) : vertexAttribute;
	vertexAttribute = pMesh->GetElementTangentCount()     > 0 ? (eVertexAttribute)( eVA_TANGENT | vertexAttribute ) : vertexAttribute;
	vertexAttribute = pMesh->GetElementUVCount()          > 0 ? (eVertexAttribute)( eVA_UV      | vertexAttribute ) : vertexAttribute;
	vertexAttribute = pMesh->GetElementBinormalCount()    > 0 ? ( eVertexAttribute )( eVA_BINORMAL | vertexAttribute ) : vertexAttribute;
	vertexAttribute = pMesh->GetElementVertexColorCount() > 0 ? ( eVertexAttribute )( eVA_VERTEXCOLOR | vertexAttribute ) : vertexAttribute;

	return vertexAttribute;
}

void import_material_info( const FbxMesh* pMesh , SIMesh& siMesh , const SArray<FbxSurfaceMaterial*>& FbxMaterials )
{
	FbxNode* pNode    = pMesh->GetNode();
	int MaterialCount = pNode->GetMaterialCount();
	if( !MaterialCount )	return;

	SArray<int> MatIndexList;
	MatIndexList.Reserve( MaterialCount );

	for( int i = 0; i < MaterialCount; i++ )
	{
		FbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial( i );
		int index = -1;
		for( int k = 0 , num = FbxMaterials.GetSize(); k < num; k++ )
		{
			if( pSurfaceMaterial == FbxMaterials[k] )
			{
				index = k;
				break;
			}
		}

		MatIndexList.PushBack( index );
	}

	// just support one material index per vertex
	const FbxGeometryElementMaterial* pMaterialLayer = pMesh->GetElementMaterial();
	auto materialRefMode = pMaterialLayer ? pMaterialLayer->GetReferenceMode() : FbxGeometryElement::eByControlPoint;
	auto materialMapMode = pMaterialLayer ? pMaterialLayer->GetMappingMode() : FbxGeometryElement::eIndexToDirect;

	for( int iPolygonIndex = 0 , polygonCount = pMesh->GetPolygonCount(); iPolygonIndex < polygonCount; iPolygonIndex++ )
	{
		for( int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++ )
		{
			int iControlPointIndex = pMesh->GetPolygonVertex( iPolygonIndex , iVertexIndex );

			switch( materialMapMode )
			{
				case FbxGeometryElement::eByControlPoint:
				{
					switch( materialRefMode )
					{
						case FbxGeometryElement::eIndexToDirect:
						{
							int matIdx = pMaterialLayer->GetIndexArray().GetAt( iControlPointIndex );
							if( matIdx >= 0 && matIdx < ( int )MatIndexList.GetSize() )
							{
								siMesh.SetMaterialIndex( iPolygonIndex , MatIndexList[matIdx] );
							}
							break;
						}

						default:
							break;
					}
					break;
				}

				case FbxGeometryElement::eByPolygon:
				{
					switch( materialRefMode )
					{
						case FbxGeometryElement::eIndexToDirect:
						{
							int matIdx = pMaterialLayer->GetIndexArray().GetAt( iPolygonIndex );
							if( matIdx >= 0 && matIdx < ( int )MatIndexList.GetSize() )
							{
								siMesh.SetMaterialIndex( iPolygonIndex , MatIndexList[matIdx] );
							}
							break;
						}
						
						default:
							break;
					}
					break;
				}

				case FbxGeometryElement::eAllSame:
				{
					switch (materialRefMode)
					{
						case FbxGeometryElement::eIndexToDirect:
						{
							int matIndex = pMaterialLayer->GetIndexArray().GetAt( 0 );
							if( matIndex >= 0 && matIndex < ( int )MatIndexList.GetSize() )
							{
								siMesh.SetMaterialIndex( iPolygonIndex , MatIndexList[matIndex] );
							}
							break;
						}

						default:
							break;
					}
				}

				default:
					break;
			}
		}
	}
}

void import_geometry_topology( FbxMesh* pMesh , SIMesh& siMesh )
{
	FbxVector4* pControlPoints = pMesh->GetControlPoints();
	for( int iControlPointIndex = 0 , iControlPointCount = pMesh->GetControlPointsCount(); iControlPointIndex < iControlPointCount; iControlPointIndex++ )
	{
		const FbxVector4& p = pControlPoints[iControlPointIndex];
		SIVertex& vertex = siMesh.AddVertex();
		vertex.Pos = SVector3f( ( float )p[0] , ( float )p[1] , ( float )p[2] );
	}

	for( int iPolygonIndex = 0 , iPolygonCount = pMesh->GetPolygonCount(); iPolygonIndex < iPolygonCount; iPolygonIndex++ )
	{
		SITriangle& tri = siMesh.AddTriangle();

		for( int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++ )
		{
			tri.vertexID[iVertexIndex] = pMesh->GetPolygonVertex( iPolygonIndex , iVertexIndex );
		}
	}
}

void import_vertex_info( FbxMesh* pMesh , SIMesh& siMesh )
{
	for( int iPolygonIndex = 0 ,polygonVertexIdx = 0, iPolygonCount = pMesh->GetPolygonCount(); iPolygonIndex < iPolygonCount; iPolygonIndex++ )
	{
		for( int iVertexIndex = 0; iVertexIndex < 3; iVertexIndex++ )
		{
			int iControlPointIndex = pMesh->GetPolygonVertex( iPolygonIndex , iVertexIndex );

			FbxGeometryElementNormal* pNormalLayer = pMesh->GetElementNormal();
			if( pNormalLayer )
			{
				switch( pNormalLayer->GetMappingMode() )
				{
					case FbxGeometryElement::eByControlPoint:
					{
						switch( pNormalLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& normal = pNormalLayer->GetDirectArray().GetAt( iControlPointIndex );
								siMesh.SetVertexNormal( iControlPointIndex , SVector3f( ( float )normal[0] , ( float )normal[1] , ( float )normal[2] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int normalIdx = pNormalLayer->GetIndexArray().GetAt( iControlPointIndex );
								const auto& normal = pNormalLayer->GetDirectArray().GetAt( normalIdx );
								siMesh.SetVertexNormal( iControlPointIndex , SVector3f( ( float )normal[0] , ( float )normal[1] , ( float )normal[2] ) );
								break;
							}

							default:
								break;
						}
						break;
					}

					case FbxGeometryElement::eByPolygonVertex:
					{
						switch( pNormalLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& normal = pNormalLayer->GetDirectArray().GetAt( polygonVertexIdx );
								siMesh.SetVertexNormal( iControlPointIndex , SVector3f( ( float )normal[0] , ( float )normal[1] , ( float )normal[2] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int normalIdx = pNormalLayer->GetIndexArray().GetAt( polygonVertexIdx );
								const auto& normal = pNormalLayer->GetDirectArray().GetAt( normalIdx );
								siMesh.SetVertexNormal( iControlPointIndex , SVector3f( ( float )normal[0] , ( float )normal[1] , ( float )normal[2] ) );
								break;
							}

							default:
								break;
						}
						break;
					}

					default:
						break;
				}
			}

			FbxGeometryElementTangent* pTangentLayer = pMesh->GetElementTangent();
			if( pTangentLayer )
			{
				switch( pTangentLayer->GetMappingMode() )
				{
					case FbxGeometryElement::eByControlPoint:
					{
						switch( pTangentLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& tangent = pTangentLayer->GetDirectArray().GetAt( iControlPointIndex );
								siMesh.SetVertexTangent( iControlPointIndex , SVector4f( ( float )tangent[0] , ( float )tangent[1] , ( float )tangent[2] , ( float )tangent[3] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int tangentIdx = pTangentLayer->GetIndexArray().GetAt( iControlPointIndex );
								const auto& tangent = pTangentLayer->GetDirectArray().GetAt( tangentIdx );
								siMesh.SetVertexTangent( iControlPointIndex , SVector4f( ( float )tangent[0] , ( float )tangent[1] , ( float )tangent[2] , ( float )tangent[3] ) );
								break;
							}

							default:
								break;
						}
						break;
					}

					case FbxGeometryElement::eByPolygonVertex:
					{
						switch( pTangentLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& tangent = pTangentLayer->GetDirectArray().GetAt( polygonVertexIdx );
								siMesh.SetVertexTangent( iControlPointIndex , SVector4f( ( float )tangent[0] , ( float )tangent[1] , ( float )tangent[2] , ( float )tangent[3] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int tangentIdx = pTangentLayer->GetIndexArray().GetAt( polygonVertexIdx );
								const auto& tangent = pTangentLayer->GetDirectArray().GetAt( tangentIdx );
								siMesh.SetVertexTangent( iControlPointIndex , SVector4f( ( float )tangent[0] , ( float )tangent[1] , ( float )tangent[2] , ( float )tangent[3] ) );
								break;
							}

							default:
								break;
						}
						break;
					}

					default:
						break;
				}
			}

			FbxGeometryElementBinormal* pBinormalLayer = pMesh->GetElementBinormal();
			if( pBinormalLayer )
			{
				switch( pBinormalLayer->GetMappingMode() )
				{
					case FbxGeometryElement::eByControlPoint:
					{
						switch( pBinormalLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& binormal = pBinormalLayer->GetDirectArray().GetAt( iControlPointIndex );
								siMesh.SetVertexBinormal( iControlPointIndex , SVector3f( ( float )binormal[0] , ( float )binormal[1] , ( float )binormal[2] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int binormalIdx = pBinormalLayer->GetIndexArray().GetAt( iControlPointIndex );
								const auto& binormal = pBinormalLayer->GetDirectArray().GetAt( binormalIdx );
								siMesh.SetVertexBinormal( iControlPointIndex , SVector3f( ( float )binormal[0] , ( float )binormal[1] , ( float )binormal[2] ) );
								break;
							}

							default:
								break;
						}
						break;
					}

					case FbxGeometryElement::eByPolygonVertex:
					{
						switch( pBinormalLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& binormal = pBinormalLayer->GetDirectArray().GetAt( polygonVertexIdx );
								siMesh.SetVertexBinormal( iControlPointIndex , SVector3f( ( float )binormal[0] , ( float )binormal[1] , ( float )binormal[2] ) );
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								int binormalIdx = pBinormalLayer->GetIndexArray().GetAt( polygonVertexIdx );
								const auto& binormal = pBinormalLayer->GetDirectArray().GetAt( binormalIdx );
								siMesh.SetVertexBinormal( iControlPointIndex , SVector3f( ( float )binormal[0] , ( float )binormal[1] , ( float )binormal[2] ) );
								break;
								break;
							}

							default:
								break;
						}
						break;
					}

					default:
						break;
				}
			}

			FbxGeometryElementVertexColor* pVertexColorLayer = pMesh->GetElementVertexColor();
			if( pVertexColorLayer )
			{
				switch( pVertexColorLayer->GetMappingMode() )
				{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch( pVertexColorLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& color = pVertexColorLayer->GetDirectArray().GetAt( iControlPointIndex );
								siMesh.SetVertexColor( iControlPointIndex , SVector3f( ( float )color[0] , ( float )color[1] , ( float )color[2] ) );
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								int idx = pVertexColorLayer->GetIndexArray().GetAt( iControlPointIndex );
								const auto& color = pVertexColorLayer->GetDirectArray().GetAt( idx );
								siMesh.SetVertexColor( iControlPointIndex , SVector3f( ( float )color[0] , ( float )color[1] , ( float )color[2] ) );
								break;
							}
							default:
								break;
						}
						break;
					case FbxGeometryElement::eByPolygonVertex:
					{
						switch( pVertexColorLayer->GetReferenceMode() )
						{
							default:
								break;
							case FbxGeometryElement::eDirect:
							{
								const auto& color = pVertexColorLayer->GetDirectArray().GetAt( polygonVertexIdx );
								siMesh.SetVertexColor( iControlPointIndex , SVector3f( ( float )color[0] , ( float )color[1] , ( float )color[2] ) );
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								int idx = pVertexColorLayer->GetIndexArray().GetAt( polygonVertexIdx );
								const auto& color = pVertexColorLayer->GetDirectArray().GetAt( idx );
								siMesh.SetVertexColor( iControlPointIndex , SVector3f( ( float )color[0] , ( float )color[1] , ( float )color[2] ) );
								break;
							}
						}
						break;
					}
				}
			}

			FbxGeometryElementUV* pUVLayer = pMesh->GetElementUV();
			if( pUVLayer )
			{
				switch( pUVLayer->GetMappingMode() )
				{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
					{
						switch( pUVLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& uv = pUVLayer->GetDirectArray().GetAt( iControlPointIndex );
								siMesh.SetVertexUV( iControlPointIndex , SVector2f( ( float )uv[0] , ( float )uv[1] ) );
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								int idx = pUVLayer->GetIndexArray().GetAt( iControlPointIndex );
								const auto& uv = pUVLayer->GetDirectArray().GetAt( idx );
								siMesh.SetVertexUV( iControlPointIndex , SVector2f( ( float )uv[0] , ( float )uv[1] ) );
								break;
							}
							default:
								break;
						}
						break;
					}
					case FbxGeometryElement::eByPolygonVertex:
					{
						int uvIndex = pMesh->GetTextureUVIndex( iPolygonIndex , iVertexIndex );
						switch( pUVLayer->GetReferenceMode() )
						{
							case FbxGeometryElement::eDirect:
							{
								const auto& uv = pUVLayer->GetDirectArray().GetAt( uvIndex );
								siMesh.SetVertexUV( iControlPointIndex , SVector2f( ( float )uv[0] , ( float )uv[1] ) );
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								int idx = pUVLayer->GetIndexArray().GetAt( uvIndex );
								const auto& uv = pUVLayer->GetDirectArray().GetAt( idx );
								siMesh.SetVertexUV( iControlPointIndex , SVector2f( ( float )uv[0] , ( float )uv[1] ) );
								break;
							}
							default:
								break;
						}
						break;
					}
				}
			}
			polygonVertexIdx++;
		}
	}
}

void import_geometry( FbxNode* pRootNode , const SArray<FbxSurfaceMaterial*>& FbxMaterials , FbxGeometryConverter& converter , SIScene& scene )
{
	SArray<FbxNode*> meshNodeList;
	import_mesh_node( pRootNode , meshNodeList );

	SArray<FbxNode*> triMeshNode;
	for( int i = 0 , meshCount = meshNodeList.GetSize(); i < meshCount; i++ )
	{
		FbxNode* pNode = meshNodeList[i];
		FbxMesh* pMesh = ( FbxMesh* )pNode->GetNodeAttribute();
		if( triangluate( pMesh , converter ) )	triMeshNode.PushBack( pNode );
	}

	for( int i = 0 , meshCount = triMeshNode.GetSize(); i < meshCount; i++ )
	{
		FbxNode* pNode = triMeshNode[i];
		FbxMesh* pMesh = ( FbxMesh* )pNode->GetNodeAttribute();
		eVertexAttribute vertexAttribute = import_vertex_attribute( pMesh );

		SIMesh& siMesh = scene.AddMesh( pNode->GetName() , vertexAttribute );
		import_geometry_topology( pMesh , siMesh );
		import_material_info( pMesh , siMesh , FbxMaterials );
		import_vertex_info( pMesh , siMesh );
	}
}

void import_material( FbxNode* pNode , SArray<FbxSurfaceMaterial*>& FbxMaterial , SIScene& scene )
{
	for( int i = 0, iMaterialCount = pNode->GetMaterialCount(); i < iMaterialCount; i++ )
	{
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial( i );
		
		bool existMaterial = false;
		for( int k = 0; k < ( int )FbxMaterial.GetSize(); k++ )
		{
			if( FbxMaterial[k] == pMaterial )
			{
				existMaterial = true;
				break;
			}
		}
		if( existMaterial )	continue;

		SIMaterial& siMat = scene.AddMaterial( pMaterial->GetName() );
		FbxMaterial.PushBack( pMaterial );

		int iLayerTextureIndex = 0;
		FBXSDK_FOR_EACH_TEXTURE( iLayerTextureIndex )
		{
			FbxProperty lProperty = pMaterial->FindProperty( FbxLayerElement::sTextureChannelNames[iLayerTextureIndex] );
			if( lProperty.IsValid() )
			{
				int iTextureCount = lProperty.GetSrcObjectCount<FbxTexture>();

				for( int iTextureIndex = 0; iTextureIndex < iTextureCount; iTextureIndex++ )
				{
					// ²»¿¼ÂÇLayeredTexture
					FbxTexture* pTexture = lProperty.GetSrcObject<FbxTexture>();
					if( pTexture )
					{
						FbxFileTexture* pFileTexture = FbxCast<FbxFileTexture>( pTexture );
						const char* szRelativeName = pFileTexture->GetRelativeFileName();

						switch( FbxGeometryElement::eTextureDiffuse + iLayerTextureIndex )
						{
							case FbxGeometryElement::eTextureDiffuse:
							{
								siMat.m_Textures[eTT_Diffuse] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureNormalMap:
							{
								siMat.m_Textures[eTT_Normal] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureEmissive:
							{
								siMat.m_Textures[eTT_Emissive] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureSpecular:
							{
								siMat.m_Textures[eTT_Specular] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureShininess:
							{
								siMat.m_Textures[eTT_Roughness] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureBump:
							{
								siMat.m_Textures[eTT_Bump] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureDisplacement:
							{
								siMat.m_Textures[eTT_Displacement] = szRelativeName;
								break;
							}

							case FbxGeometryElement::eTextureReflection:
							{
								siMat.m_Textures[eTT_Anisotropy] = szRelativeName;
								break;
							}

							default:
								break;
						}
					}
				}
			}
		}
		break;
	}

	for( int i = 0 , count = pNode->GetChildCount(); i < count; i++ )
	{
		import_material( pNode->GetChild( i ) , FbxMaterial , scene );
	}
}

bool fbx_importer( const char* filename , SIScene& siScene )
{
	// Create the FBX SDK Manager
	FbxManager* lSdkManager = FbxManager::Create();
	if( !lSdkManager )	SLog::Error( "Unable to Create FBX Manager!" );

	// Create an IOSettings object. IOSROOT is defined in Fbxiosetting.h
	FbxIOSettings* ios = FbxIOSettings::Create( lSdkManager , IOSROOT );
	lSdkManager->SetIOSettings( ios );

	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_MATERIAL        , true );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_TEXTURE         , true );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_LINK            , false );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_SHAPE           , true );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_GOBO            , false );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_ANIMATION       , true );
	( *( lSdkManager->GetIOSettings() ) ).SetBoolProp( IMP_FBX_GLOBAL_SETTINGS , true );

	// Create an importer
	FbxImporter* pImporter = FbxImporter::Create( lSdkManager , "" );
	bool bImportStatus = pImporter->Initialize( filename , -1 , lSdkManager->GetIOSettings() );

	if( !bImportStatus )
	{
		SLog::Error( "FbxImporter::Initialize() Failed:%s" , pImporter->GetStatus().GetErrorString() );
		if( pImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion )
		{
			int sdkMajor  = 0,  sdkMinor  = 0,	sdkRevision  = 0;
			int fileMajor = 0 , fileMinor = 0 , fileRevision = 0;
			FbxManager::GetFileFormatVersion( sdkMajor , sdkMinor , sdkRevision );
			pImporter->GetFileVersion( fileMajor , fileMinor , fileRevision );
			SLog::Error( "FBX file format version for this FBX SDK is %d.%d.%d" , sdkMajor , sdkMinor , sdkRevision );
			SLog::Error( "FBX File format version for file '%s' is %d.%d.%d" , filename , fileMajor , fileMinor , fileRevision );
		}
		return false;
	}

	// Create a new scene so it can be populated by the imported file
	FbxScene* pScene = FbxScene::Create( lSdkManager , "myScene" );

	// Import the contents of the file into the scene
	pImporter->Import( pScene );

	// The file has been imported; we can get rid of the importer
	pImporter->Destroy();

	FbxAxisSystem axis( FbxAxisSystem::DirectX );
	axis.ConvertScene( pScene );

	FbxGeometryConverter geometryConverter( lSdkManager );

	siScene.SetFilename( filename );
	SArray<FbxSurfaceMaterial*> fbxMaterials;
	import_material( pScene->GetRootNode() , fbxMaterials , siScene );
	import_geometry( pScene->GetRootNode() , fbxMaterials , geometryConverter , siScene );

	lSdkManager->Destroy();

	return true;
}

void fbx_exporter( const char* filename )
{

}