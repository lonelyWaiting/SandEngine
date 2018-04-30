#include "SandEnginePCH.h"
#include "SResourceManager.h"
#include "SResource.h"

void SResourceManager::RegisterResource( SResource * resource )
{
	m_resourceList.PushBack( resource );
}