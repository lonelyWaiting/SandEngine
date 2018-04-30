#include "SandBase/SandBasePCH.h"
#include "SSmartPointer.h"

template<class T>
SSmartPointer<T>::SSmartPointer()
{
	m_instance = nullptr;
}

template<class T>
SSmartPointer<T>::SSmartPointer( T* instance )
{
	m_instance = instance;
	if( m_instance )	m_instance->Increase();
}

template<class T>
SSmartPointer<T>::SSmartPointer( const SSmartPointer<T>& other )
{}

template<class T>
SSmartPointer<T>::~SSmartPointer()
{
	m_instance->Release();
	if( !m_instance->GetRefCount() )	SAFE_RELEASE( m_instance );
}

template<class T>
SSmartPointer<T>& SSmartPointer<T>::operator=( const SSmartPointer<T>& other )
{
	if( m_instance )	m_instance->Release();

	m_instance = other.GetPointer();
	if( m_instance )	m_instance->Increase();
}

template<class T>
T * SSmartPointer<T>::operator->()
{
	return m_instance;
}

template<class T>
T * SSmartPointer<T>::GetPointer()
{
	return m_instance;
}

template<class T>
int SSmartPointer<T>::GetRefCount() const
{
	return m_refcount;
}