#pragma once

#include "RemoveReference.h"
#include "TEnableIf.h"

// MoveTemp will cast a reference to an rvalue reference.
// this is equaivalent of std::move.
template<typename T>
__forceinline typename TRemoveReference<T>::Type&& MoveTemp( T&& Obj )
{
	return ( typename TRemoveReference<T>::Type&& )Obj;
}

// CopyTemp will enforce the creation of an rvalue which can bind to rvalue reference parameters.
// Unlike MoveTemp, the source object will never be modifed. (i.e. a copy will be made)
// There is no std:: equivalent.
template<typename T>
__forceinline T CopyTemp( T& val )
{
	return const_cast< const T& >( val );
}

template<typename T>
__forceinline T CopyTemp( const T& val )
{
	return val;
}

template<typename T>
__forceinline T&& CopyTemp( T&& val )
{
	return MoveTemp( val );
}

// Forward will cast a reference to an rvalue reference.
// This is equivalent of std::forward.
template<typename T>
inline T&& Forward( typename TRemoveReference<T>::Type&& Obj )
{
	return ( T&& )Obj;
}

template<typename T>
inline T&& Forward( typename TRemoveReference<T>::Type& Obj )
{
	return ( T&& )Obj;
}

/*
A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-base swap.
*/
template<typename T>
struct TUseBitwiseSwap
{
	enum
	{
		Value = !TOrValue < __is_enum( T ) , TIsPointer<T> , TIsArithmetic<T>>::Value
	};
};

template<typename T>
inline typename TEnableIf<TUseBitwiseSwap<T>::Value>::Type Swap( T& A , T& B )
{
	T Temp;
	memcpy( &Temp , &A , sizeof( T ) );
	memcpy( &A , &B , sizeof( T ) );
	memcpy( &B , &Temp , sizeof( T ) );
}


template<typename T>
inline typename TEnableIf < !TUseBitwiseSwap<T>::Value >::Type Swap( T& A , T& B )
{
	T Temp = MoveTemp( A );
	A = MoveTemp( B );
	B = MoveTemp( Temp );
}

template<class T>
inline void Exchange( T& A , T& B )
{
	Swap( A , B );
}

/**
* Returns the same type passed to it.  This is useful in a few cases, but mainly for inhibiting template argument deduction in function arguments, e.g.:
*
* template <typename T>
* void Func1(T Val); // Can be called like Func(123) or Func<int>(123);
*
* template <typename T>
* void Func2(typename TIdentity<T>::Type Val); // Must be called like Func<int>(123)
*/
template<typename T>
struct TIdentity
{
	typedef T Type;
};

template<typename T , suInt32 N>
char( &ArrayCountHelper( const T( &)[N] ) )[N];

#define ARRAY_COUNT(array)	(sizeof(ArrayCountHelper(array)) + 0)