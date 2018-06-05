#include "SandEnginePCH.h"
#include "SResource.h"
#include "SResourceManager.h"
#include "Application/SRenderer.h"

SResource::SResource()
{
	m_resourceFlag = eRF_None;
}

SResource::SResource( const char * name , SResourceManager& manager )
{
	manager.RegisterResource( this );
	SetFileName( name );
}

const SString & SResource::GetFilename()
{
	return m_name;
}

void SResource::SetFileName( const char * name )
{
	m_name = name;
}

void SResource::SetResourceFlag( eResourceFlag flag )
{
	m_resourceFlag |= flag;
}

int SResource::GetResourceFlag() const
{
	return m_resourceFlag;
}

bool SResource::HasFlag( eResourceFlag flag ) const
{
	return (m_resourceFlag & flag) == flag;
}

void SResource::RemoveResourceFlag(eResourceFlag flag)
{
	m_resourceFlag &= ~flag;
}