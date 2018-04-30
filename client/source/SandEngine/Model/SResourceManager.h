#pragma once

#include "SResource.h"
#include "SandBase/Vector/SArray.h"

class SResource;

class SResourceManager
{
public:
	static SResourceManager & Get()
	{
		static SResourceManager manager;
		return manager;
	}

	void RegisterResource( SResource* resource );

protected:
	SArray<SResource*> m_resourceList;
};