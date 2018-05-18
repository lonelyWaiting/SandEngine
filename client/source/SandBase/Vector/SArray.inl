#include "SArray.h"
#include "SandBase/Template/utils.h"
#include "SandBase/Math/SMath.h"
#include "SandBase/Sort/Sort.h"
#include <iostream>
#include <cassert>

template<typename T>
void SArray<T>::CopyToEmpty( const T* pData , int size )
{
	Resize(size);
	ConstructItems( m_pData , pData , m_iSize );
}

template<typename T>
SArray<T>::SArray()
{
	m_iSize = 0;
	m_iCapacity = 0;
	m_pData = nullptr;
}

template<class T>
inline SArray<T>::SArray( std::initializer_list<T> InitList )
{
	CopyToEmpty( InitList.begin() , (sInt32)InitList.size() );
}

template<typename T>
SArray<T>::SArray( const SArray& Other )
{
	CopyToEmpty( Other.GetData() , Other.GetSize() );
}

template<class T>
inline SArray<T>& SArray<T>::operator=( std::initializer_list<T> InitList )
{
	DestructItems( GetData() , m_iSize );
	CopyToEmpty( InitList.begin() , (sInt32)InitList.size() );

	return *this;
}

template<class T>
inline SArray<T>& SArray<T>::operator=( const SArray<T>& Other )
{
	if( this != &Other )
	{
		DestructItems( GetData() , m_iSize );

		CopyToEmpty( Other.GetData() , Other.GetSize() );
	}

	return *this;
}

template<typename T>
SArray<T>::~SArray()
{
	DestructItems( GetData() , m_iSize );
	m_pData = nullptr;
	m_iSize = 0;
	m_iCapacity = 0;
}

template<typename T>
suInt32 SArray<T>::GetSize() const
{
	return m_iSize;
}

template<typename T>
suInt32 SArray<T>::GetCapacity() const
{
	return m_iCapacity;
}

template<typename T>
bool SArray<T>::IsEmpty() const
{
	return m_iSize == 0;
}

template<typename T>
T* SArray<T>::GetData()
{
	return m_pData;
}

template<typename T>
const T* SArray<T>::GetData() const
{
	return m_pData;
}

template<typename T>
void SArray<T>::Reserve( suInt32 NewCapacity )
{
	if( NewCapacity > m_iCapacity )
	{
		m_pData = m_pData ? (T*)realloc(m_pData, NewCapacity * sizeof(T)) : (T*)malloc(NewCapacity * sizeof(T));
		m_iCapacity = NewCapacity;
	}
}

template<typename T>
T& SArray<T>::operator[]( suInt32 pos )
{
	assert( pos >= 0 && pos < m_iSize );

	return m_pData[pos];
}

template<typename T>
const T& SArray<T>::operator[]( suInt32 pos ) const
{
	assert( pos >= 0 && pos < m_iSize );

	return m_pData[pos];
}

template<typename T>
void SArray<T>::PushBack( const T& Item )
{
	assert( &Item < GetData() || &Item >= ( GetData() + m_iCapacity ) );

	if (m_iSize + 1 > m_iCapacity)
	{
		Reserve( ( suInt32 )( ( m_iSize + 1 ) * 1.5f ) );
	}

	new (GetData() + m_iSize)T(Item);
	
	m_iSize += 1;
}

template<typename T>
void SArray<T>::Clear()
{
	m_iSize = 0;
}

template<class T>
inline bool SArray<T>::Contains( const T & Item ) const
{
	for( const T* pData = GetData() , *pDataEnd = pData + m_iSize; pData != pDataEnd; ++pData )
	{
		if( *pData == Item )
		{
			return true;
		}
	}

	return false;
}

template<typename T>
void SArray<T>::Insert( const SArray<T>& Items , const sInt32 Index )
{
	assert( this->GetData() != Items.GetData() );

	InsertUninitialized( Index , Items.GetSize() );

	for( suInt32 iStartIndex = 0; iStartIndex < Items.GetSize(); iStartIndex++ )
	{
		new ( m_pData + iStartIndex + Index )T( Items[iStartIndex] );
	}
}

template<typename T>
void SArray<T>::InsertUninitialized( suInt32 index , suInt32 Count /* = 1 */ )
{
	if( index >= m_iSize )	return;

	const suInt32 oldSize= m_iSize;
	if( m_iSize + Count > m_iCapacity )
	{
		Resize( m_iSize + Count );
	}
	else
	{
		m_iSize += Count;
	}

	if( oldSize > 0 )
	{
		T* pData = GetData() + oldSize - 1;
		T* pDestData = GetData() + m_iSize - 1;

		int reallocCount = oldSize - index;

		while( reallocCount )
		{
			new ( pDestData )T( *pData );
			( pData-- )->T::~T();
			--pDestData;
			--reallocCount;
		}
	}
}

template<typename T>
void SArray<T>::InsertZeroed( suInt32 Index , suInt32 Count /* = 1 */ )
{
	InsertUninitialized( Index , Count );

	if( Index >= 0 && Index < m_iSize )
	{
		for( suInt32 Start = Index , End = Count + Index; Start < End; Start++ )
		{
			*( m_pData + Start ) = T();
		}
	}
}

template<typename T>
void SArray<T>::Insert( const T* Ptr , const suInt32 Count , const suInt32 Index )
{
	assert( Ptr != nullptr && Index >= 0 && Index < m_iSize);

	InsertUninitialized( Index , Count );

	if(Index < m_iSize )
	{
		ConstructItems<T>( GetData() + Index , Ptr , Count );
	}
}

template<typename T>
void SArray<T>::Insert( suInt32 Index , const T& value )
{
	InsertUninitialized( Index , 1);
	if( Index >= 0 && Index < m_iSize )
	{
		ConstructItems( GetData() + Index , &value , 1 );
	}
}

template<typename T>
void SArray<T>::RemoveAtImpl( suInt32 Index , suInt32 Count )
{
	if (Index < 0 || Index > m_iSize)	return;

	Count = SMath::Min(Count, m_iSize - Index);

	if (Count <= 0)	return;

	DestructItems(GetData() + Index, Count);

	if (int iNumToMove = m_iSize - Index - Count)
	{
		std::memmove(GetData() + Index, GetData() + Index + Count, sizeof(T) * iNumToMove);
	}

	m_iSize -= Count;
}

template<typename T>
void SArray<T>::Remove( sInt32 Index , suInt32 Count )
{
	RemoveAtImpl( Index , Count );
}

template<typename T>
void SArray<T>::RemoveIndex( sInt32 Index )
{
	RemoveAtImpl( Index , 1 );
}

template<typename T>
sInt32 SArray<T>::RemoveValue( const T& Item )
{
	assert( &Item < GetData() || &Item >= ( GetData() + m_iCapacity ) );

	return RemoveAll( [&Item]( const T& Element )
	{
		return Element == Item;
	} );
}

template<class T>
template<class PREDICATE_CLASS>
inline sInt32 SArray<T>::RemoveAll( const PREDICATE_CLASS & Predicate )
{
	const suInt32 OriginalNum = m_iSize;
	if (OriginalNum == 0)	return 0;

	suInt32 WriteIndex = 0;
	suInt32 ReadIndex = 0;
	bool NotMatch = !Predicate( GetData()[ReadIndex] );
	do 
	{
		suInt32 RunStartIndex = ReadIndex++;
		while( ReadIndex < OriginalNum && NotMatch == !Predicate( GetData()[ReadIndex] ) )
		{
			ReadIndex++;
		}

		int RunLength = ReadIndex - RunStartIndex;
		if( NotMatch )
		{
			// this was a non-matching run,we need to remove it
			if( WriteIndex != RunStartIndex )
			{
				std::memmove( &GetData()[WriteIndex] , &GetData()[RunStartIndex] , sizeof( T ) * RunLength );
			}

			WriteIndex += RunLength;
		}
		else
		{
			// this was a matching run, delete it
			DestructItems( &GetData()[RunStartIndex] , RunLength );
		}

		// alternate search.
		NotMatch = !NotMatch;
	} while( ReadIndex < OriginalNum );

	m_iSize = WriteIndex;

	return OriginalNum - m_iSize;
}

template<typename T>
inline sInt32 SArray<T>::Find( const T& Item ) const
{
	const T* Start = GetData();
	for( const T* pData = Start , *pEnd = pData + m_iSize; pData != pEnd; pData++ )
	{
		if( *pData == Item )
		{
			return static_cast< sInt32 >( pData - Start );
		}
	}

	return INDEX_NONE;
}

template<class T>
template<typename Predicate>
inline sInt32 SArray<T>::FindFirstByPredicate( Predicate Pred )
{
	for( T* pData = GetData() , *pDataEnd = pData + m_iSize; pData != pDataEnd; ++pData )
	{
		if( Pred( *pData ) )
		{
			return ( sInt32 )( pData - GetData() );
		}
	}

	return INDEX_NONE;
}

template<class T>
template<typename Predicate>
inline SArray<suInt32> SArray<T>::FindListByPredicate( Predicate Pred ) const
{
	SArray<suInt32> FilterResult;

	for( const T* pData = GetData() , *pDataEnd = pData + m_iSize; pData != pDataEnd; ++pData )
	{
		if( Pred( *pData ) )
		{
			FilterResult.PushBack( ( suInt32 )( pData - GetData() ) );
		}
	}
	return FilterResult;
}

template<class T>
inline T SArray<T>::Pop( )
{
	T Result = GetData()[m_iSize - 1];
	Remove(m_iSize - 1, 1);
	return Result;
}

template<class T>
inline T& SArray<T>::Peek()
{
	return m_pData[m_iSize - 1];
}

template<class T>
inline void SArray<T>::Sort()
{
	::Sort( GetData() , m_iSize );
}


template<class T>
template<class PREDICATE_CLASS>
inline void SArray<T>::Sort( const PREDICATE_CLASS & Predicate )
{
	::Sort( GetData() , m_iSize , Predicate );
}

template<typename T>
void SArray<T>::Resize(suInt32 newSize)
{
	if (newSize == m_iSize)	return;

	if (newSize > m_iCapacity)
	{
		m_iCapacity = newSize;
		m_iSize		= newSize;

		m_pData = m_pData ? (T*)::realloc(m_pData, m_iCapacity * sizeof(T)) : (T*)::malloc(m_iCapacity * sizeof(T));
	}
	else
	{
		if (newSize == 0)
		{
			::free(m_pData);
			m_pData = nullptr;
		}
		else
		{
			m_pData = m_pData ? (T*)::realloc(m_pData, newSize * sizeof(T)) : (T*)::malloc(newSize * sizeof(T));
		}
	}

	m_iSize = newSize;
}  

template<typename T>
void SArray<T>::Shrink()
{
	if (m_iCapacity > m_iSize && m_pData)
	{
		m_pData = (T*)::realloc(m_pData, m_iSize * sizeof(T));
		m_iCapacity = m_iSize;
	}
}

template<class T>
inline void SArray<T>::Swap( SArray<T>& other )
{
	SMath::swap( m_pData , other.m_pData );
	SMath::swap( m_iCapacity , other.m_iCapacity );
	SMath::swap( m_iSize , other.m_iSize );
}