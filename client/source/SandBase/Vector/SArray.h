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
	T& operator[]( suInt32 pos );
	const T& operator[]( suInt32 pos ) const;

public:
	suInt32		GetSize() const;
	suInt32		GetCapacity() const;
	bool		IsEmpty() const;
	T*			GetData();
	const T*	GetData() const;
	void		Reserve( suInt32 newCapacity );
	void		PushBack( const T& element );
	T			Pop();
	T&			Peek();
	void		Clear();
	bool		Contains( const T& Item ) const;
	void		Insert( suInt32 pos , const T& value );
	void		InsertZeroed( suInt32 Index , suInt32 Count = 1 );
	void		Insert( const SArray<T>& Items , const sInt32 Index );
	void		Insert( const T* Ptr , const suInt32 Count , const suInt32 Index );
	void		RemoveIndex( sInt32 Index );
	void		Remove( sInt32 Index , suInt32 Count = 1 );
	sInt32		RemoveValue( const T& Item );
	template<class PREDICATE_CLASS>
	sInt32		RemoveAll( const PREDICATE_CLASS& Predicate );
	
	sInt32			Find( const T& Item ) const;	
	template<typename Predicate>
	sInt32				FindFirstByPredicate( Predicate Pred );
	template<typename Predicate>
	SArray<suInt32>		FindListByPredicate( Predicate Pred ) const;

	// Sorts the array assuming < operator is defined for the item type
	void Sort();

	template<class PREDICATE_CLASS>
	void Sort( const PREDICATE_CLASS& Predicate );

	void Resize(suInt32 newSize);
	void Shrink();

	void Swap( SArray<T>& other );

protected:
	void CopyToEmpty( const T* pData , int size );
	void InsertUninitialized( suInt32 index , suInt32 count = 1 );
	void RemoveAtImpl( suInt32 Index , suInt32 Count );

public:
	T* m_pData			= nullptr;
	suInt32 m_iSize		= 0;
	suInt32 m_iCapacity	= 0;
};

#include "SArray.inl"
#endif