#pragma once

#include "SandBase/Template/TEnableIf.h"
#include "SandBase/Template/TIsTriviallyCopyConstructible.h"
#include "SandBase/Template/IsTriviallyDestructible.h"

template<typename T>
typename TEnableIf<TIsTriviallyCopyConstructible<T>::Value>::Type ConstructItems(T* pDest, const T* pSrc, int Count)
{
	std::memcpy(pDest, pSrc, sizeof(T) * Count);
}

template<typename T>
typename TEnableIf<!TIsTriviallyCopyConstructible<T>::Value>::Type ConstructItems(T* pDest, const T* pSrc, int Count)
{
	while (Count)
	{
		new (pDest)T(*pSrc);
		++pDest;
		++pSrc;
		Count--;
	}
}

template<typename T>
typename TEnableIf<TIsTriviallyDestructible<T>::Value>::Type DestructItems(T* pData, int32 Count)
{

}

template<typename T>
typename TEnableIf<!TIsTriviallyDestructible<T>::Value>::Type DestructItems(T* pData, int32 Count)
{
	while (Count)
	{
		pData->T::~T();
		++pData;
		Count--;
	}
}