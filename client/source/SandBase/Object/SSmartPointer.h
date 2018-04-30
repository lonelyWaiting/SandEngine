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
	T* operator->();
	T* GetPointer();

private:
	T*  m_instance = nullptr;
};