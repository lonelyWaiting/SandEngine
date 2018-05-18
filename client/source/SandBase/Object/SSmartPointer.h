#pragma once

template<class T>
class SSmartPointer
{
public:
	SSmartPointer();
	SSmartPointer( T* instance );
	SSmartPointer( const SSmartPointer<T>& other );
	virtual ~SSmartPointer();
	
	SSmartPointer<T>& operator=( const SSmartPointer<T>& other );
	SSmartPointer<T>& operator=( T* instance );
	T* operator->();
	T* GetPointer();
	operator T*();

private:
	T*  m_instance = nullptr;
};

#include "SSmartPointer.inl"