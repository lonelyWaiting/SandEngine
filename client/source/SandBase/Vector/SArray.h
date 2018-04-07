#ifndef SARRAY_H
#define SARRAY_H

#include <initializer_list>
#include "SandBase/SandBasePCH.h"

template<class T>
class SArray
{
public:
	SArray();

	// Initializer list constructor.
	SArray( std::initializer_list<T> InitList );

	SArray( const SArray& Other );

	SArray& operator = ( std::initializer_list<T> InitList );

	// copy assignment operator. Replaces the contents with a copy of the contents of other.
	SArray& operator=( const SArray& other );

	~SArray();

public:
	T& operator[]( uint32 pos );
	const T& operator[]( uint32 pos ) const;

public:
	uint32		GetSize() const;
	uint32		GetCapacity() const;
	bool		IsEmpty() const;
	T*			GetData();
	const T*	GetData() const;
	void		Reserve( uint32 newCapacity );
	void		PushBack( const T& element );
	T			Pop();
	T&			Peek();
	void		Clear();
	bool		Contains( const T& Item ) const;
	void		Insert( uint32 pos , const T& value );
	void		InsertZeroed( uint32 Index , uint32 Count = 1 );
	void		Insert( const SArray<T>& Items , const int32 Index );
	void		Insert( const T* Ptr , const uint32 Count , const uint32 Index );
	void		RemoveIndex( int32 Index );
	void		Remove( int32 Index , uint32 Count = 1 );
	int32		RemoveValue( const T& Item );
	template<class PREDICATE_CLASS>
	int32		RemoveAll( const PREDICATE_CLASS& Predicate );
	
	int32			Find( const T& Item ) const;	
	template<typename Predicate>
	int32				FindFirstByPredicate( Predicate Pred );
	template<typename Predicate>
	SArray<uint32>		FindListByPredicate( Predicate Pred ) const;

	// Sorts the array assuming < operator is defined for the item type
	void Sort();

	template<class PREDICATE_CLASS>
	void Sort( const PREDICATE_CLASS& Predicate );

	void Resize(uint32 newSize);
	void Shrink();

	void Swap( SArray<T>& other );

protected:
	void CopyToEmpty( const T* pData , int size );
	void InsertUninitialized( uint32 index , uint32 count = 1 );
	void RemoveAtImpl( uint32 Index , uint32 Count );

public:
	T* m_pData			= nullptr;
	uint32 m_iSize		= 0;
	uint32 m_iCapacity	= 0;
};

#include "SArray.inl"
#endif