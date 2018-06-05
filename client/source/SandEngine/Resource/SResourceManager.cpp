#include "SandEnginePCH.h"
#include "SResourceManager.h"
#include "SResource.h"

void SResourceManager::RegisterResource( SResource * resource )
{
	m_resourceList.PushBack( resource );
}

SResource* SResourceManager::FindResourceByName( const char* filename )
{
	SResource* resource = nullptr;

	for( int i = 0 , size = m_resourceList.GetSize(); i < size; i++ )
	{
		if( m_resourceList[i]->GetFilename().IsEqual( filename ) )
		{
			resource = m_resourceList[i];
			break;
		}
	}

	return resource;
}