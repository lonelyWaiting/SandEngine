#include "SandBase/SandBasePCH.h"
#include "SRefCounter.h"

void SRefCounter::AddRef()
{
	++m_refCount;
}

void SRefCounter::Release()
{
	--m_refCount;
	if( m_refCount == 0 )	delete this;
}

int SRefCounter::GetRefCount()
{
	return m_refCount;
}
