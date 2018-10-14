#pragma once

#include "SandBase/Object/SRefCounter.h"
#include "SandBase/String/SString.h"
#include "SandEngine/SandEnginePCH.h"
#include "SandBase/Object/SSmartPointer.h"
#include "SandEngine/SApiDef.h"

enum eResourceFlag
{
	eRF_None       = 0,
	eRF_Loading    = SBIT(0),
	eRF_Loaded     = SBIT(1),
	eRF_IsSteaming = SBIT(2),
};

class SResourceManager;

class SResource : public SRefCounter
{
public:
	SResource();
	SResource( const char* name , SResourceManager& manager );
	const SString& GetFilename();
	void SetFileName( const char* name );
	void SetResourceFlag( eResourceFlag flag );
	int  GetResourceFlag() const;
	void RemoveResourceFlag(eResourceFlag flag);
	bool HasFlag( eResourceFlag flag ) const;
	virtual void Ensureloaded() {}
	virtual void EnsureUnloaded() {}

private:
	SString m_name;
	int m_resourceFlag = eRF_None;
};

typedef SSmartPointer<SResource> SResourcePtr;