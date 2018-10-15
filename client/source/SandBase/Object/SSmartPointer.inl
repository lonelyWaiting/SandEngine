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
	if( m_instance )	m_instance->AddRef();
}

template<class T>
SSmartPointer<T>::SSmartPointer( const SSmartPointer<T>& other )
{
	m_instance = other.m_instance;
	if( m_instance )	m_instance->AddRef();
}

template<class T>
SSmartPointer<T>::~SSmartPointer()
{
	if( m_instance )	m_instance->Release();
}

template<class T>
SSmartPointer<T>& SSmartPointer<T>::operator=( const SSmartPointer<T>& other )
{
	if( m_instance )	m_instance->Release();

	m_instance = other.GetPointer();
	if( m_instance )	m_instance->AddRef();

	return *this;
}

template<class T>
SSmartPointer<T>& SSmartPointer<T>::operator=( T * instance )
{
	if( m_instance )	m_instance->Release();

	m_instance = instance;
	if( m_instance )	m_instance->AddRef();

	return *this;
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
const T * SSmartPointer<T>::GetPointer() const
{
	return m_instance;
}

template<class T>
inline SSmartPointer<T>::operator T*( )
{
	return m_instance;
}
