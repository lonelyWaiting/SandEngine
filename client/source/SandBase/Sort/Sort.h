#pragma once

#include "SandBase/Template/Less.h"
#include "SandBase/Template/Greater.h"
#include "SandBase/Template/TemplateBase.h"

// Helper class for dereferencing pointer types in Sort function
template<typename T, class PREDICATE_CLASS>
struct TDeferenceWrapper
{
	const PREDICATE_CLASS& Predicate;

	TDeferenceWrapper(const PREDICATE_CLASS& InPredicate)
		: Predicate(InPredicate)
	{

	}

	inline bool operator()(const T& A, const T& B) const
	{
		return Predicate(A, B);
	}
};

// Partially specialized version of the abvoe class
template<typename T, class PREDICATE_CLASS>
struct TDeferenceWrapper<T*, PREDICATE_CLASS>
{
	const PREDICATE_CLASS& Predicate;

	TDeferenceWrapper(const PREDICATE_CLASS& InPredicate)
		:Predicate(InPredicate)
	{

	}

	inline bool operator()(const T* A, const T* B) const
	{
		return Predicate(*A, *B);
	}
};

// if Predicate() return true. Exchange
// if Predicate() return false. nothing.
// for example : if Predicate is Less Compare.
// bool operator()(T& A , T& B)	{return A.member < B.member }
// then result is from small to large sequence.
// 假设Precidatey为
// bool Predicate(const T& A , const T& B)
// {
//		return A < B;
// }
template<class T, class PREDICATE_CLASS>
void SortInternal(T* First, const suInt32 Num, const PREDICATE_CLASS& Predicate)
{
	struct FStack
	{
		T* Min;
		T* Max;
	};

	// 少于两个元素，没有排序的必要.
	if (Num < 2)
	{
		return;
	}

	FStack RecursionStack[32] = { { First , First + Num - 1 } }, Current, Inner;

	for (FStack* StackTop = RecursionStack; StackTop >= RecursionStack; --StackTop)
	{
		Current = *StackTop;

	Loop:
		sInt32 Count = (sInt32)( Current.Max - Current.Min + 1 );
		if (Count <= 8)
		{
			// 改用冒泡排序
			while (Current.Max > Current.Min)
			{
				T *Max, *Item;

				for (Max = Current.Min, Item = Current.Min + 1; Item <= Current.Max; Item++)
				{
					if (Predicate(*Max, *Item))
					{
						Max = Item;
					}
				}

				Exchange(*Max, *Current.Max--);
			}
		}
		else
		{
			// 快排，交换第一个和中间元素，避免最坏情况
			Exchange(Current.Min[Count / 2], Current.Min[0]);

			// Current.Min的值作为分界线，将元素分割为三部分
			// 一部分小于等于分界线，一部分等于分界线，一部分大于等于分界线
			Inner.Min = Current.Min;
			Inner.Max = Current.Max + 1;
			for (;; )
			{
				// 找到第一个大于等于*Current.Min的数
				// 也就是说Inner.Min之前的全都小于等于*Current.Min
				while (++Inner.Min <= Current.Max && !Predicate(*Current.Min, *Inner.Min));
				// 找到第一个小于*Current.Min的数
				// 也就是说Innner.Max之后的全都大于*Current.Min
				while (--Inner.Max > Current.Min && !Predicate(*Inner.Max, *Current.Min));
				if (Inner.Min > Inner.Max)
				{
					break;
				}

				Exchange(*Inner.Min, *Inner.Max);
			}

			// 将Current.Min交换到中间去
			// 因此，现在的情况是
			// ---------1----------|------2------|-------3-----------------
			//					Inner.Max     Inner.Min
			// 2区域的值即为分界线的值，1区域的值全部小于等于分界线
			// 3区域的值全部大于等于分界线的值
			Exchange(*Current.Min, *Inner.Max);

			// 此时Inner.Max所指向元素的值等于分界线的值

			// 将元素多的一方先入栈，排序元素少的一方
			if (Inner.Max - 1 - Current.Min >= Current.Max - Inner.Min)
			{
				if (Current.Min + 1 < Inner.Max)
				{
					StackTop->Min = Current.Min;
					StackTop->Max = Inner.Max - 1;
					StackTop++;
				}

				if (Current.Max > Inner.Min)
				{
					Current.Min = Inner.Min;
					goto Loop;
				}
			}
			else
			{
				if (Current.Max > Inner.Min)
				{
					StackTop->Min = Inner.Min;
					StackTop->Max = Current.Max;
					StackTop++;
				}

				if (Inner.Max - 1 > Current.Min)
				{
					Current.Max = Inner.Max - 1;
				}
			}
		}
	}
}

// Sort elements using user defined predicate class.
// The sort is unstable, meaning that the ordering of equal items is not necessarily preversed.
template<class T, class PREDICATE_CLASS>
void Sort(T* First, const suInt32 Num, const PREDICATE_CLASS& Predicate)
{
	SortInternal(First, Num, TDeferenceWrapper<T, PREDICATE_CLASS>(Predicate));
}

// Specialized version of the above Sort function for pointers to elements.
template<class T, class PREDICATE_CLASS>
void Sort(T** First, const suInt32 Num, const PREDICATE_CLASS& Predicate)
{
	SortInternal(First, Num, TDeferenceWrapper<T*, PREDICATE_CLASS>(Predicate));
}

// Sort elements. The sort is unstable, meaning that the ordering of equal items is not necessarily preserved.
// Assumes < operator is defined for the template type.
template<class T>
void Sort(T* First, const suInt32 Num)
{
	SortInternal(First, Num, TDeferenceWrapper<T, TLess<T>>(TLess<T>()));
}

// Specialized version of the above Sort function for pointers to elements.
template<class T>
void Sort(T** First, const suInt32 Num)
{
	SortInternal(First, Num, TDeferenceWrapper<T*, TLess<T>>(TLess<T>()));
}