#pragma once

#include "SResource.h"
#include "SandBase/Vector/SArray.h"

class SResourceManager
{
public:
	SResourceManager() {}
	void RegisterResource( SResource* resource );
	SResource* FindResourceByName( const char* filename );

protected:
	SArray<SResourcePtr> m_resourceList;
};