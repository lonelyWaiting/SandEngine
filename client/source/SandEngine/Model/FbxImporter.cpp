#include "SandEnginePCH.h"
#include "FbxImporter.h"
#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <fbxsdk/fileio/fbximporter.h>

void fbx_importer( const char* filename )
{
	// Create the FBX SDK Manager
	FbxManager* lSdkManager = FbxManager::Create();

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
		std::cout << "Call to FbxImporter::Initialize() Failed" << std::endl << "Error returned: %s" << pImporter->GetStatus().GetErrorString << std::endl;
		return;
	}

	// Create a new scene so it can be populated by the imported file
	FbxScene* pScene = FbxScene::Create( lSdkManager , "myScene" );

	// Import the contents of the file into the scene
	pImporter->Import( pScene );

	// The file has been imported; we can get rid of the importer
	pImporter->Destroy();
}

void fbx_exporter( const char* filename )
{

}