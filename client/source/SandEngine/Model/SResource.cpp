#include "SandEnginePCH.h"
#include "SResource.h"
#include "SResourceManager.h"
#include "Application/SRenderer.h"

SResource::SResource( const char * name )
{
	SResourceManager::Get().RegisterResource( this );
	SetFileName( name );
}

const SString & SResource::GetFilename()
{
	return m_name;
}

void SResource::SetFileName( const char * name )
{
	m_name = name;
	SRenderer::Get().GetDevice()->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( name ) - 1 , name );
}

void SResource::SetResourceFlag( eResourceFlag flag )
{
	m_resourceFlag |= flag;
}

int SResource::GetResourceFlag() const
{
	return m_resourceFlag;
}

bool SResource::HasFlag( eResourceFlag& flag ) const
{
	return m_resourceFlag & flag;
}