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
// ����PrecidateyΪ
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

	// ��������Ԫ�أ�û������ı�Ҫ.
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
			// ����ð������
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
			// ���ţ�������һ�����м�Ԫ�أ���������
			Exchange(Current.Min[Count / 2], Current.Min[0]);

			// Current.Min��ֵ��Ϊ�ֽ��ߣ���Ԫ�طָ�Ϊ������
			// һ����С�ڵ��ڷֽ��ߣ�һ���ֵ��ڷֽ��ߣ�һ���ִ��ڵ��ڷֽ���
			Inner.Min = Current.Min;
			Inner.Max = Current.Max + 1;
			for (;; )
			{
				// �ҵ���һ�����ڵ���*Current.Min����
				// Ҳ����˵Inner.Min֮ǰ��ȫ��С�ڵ���*Current.Min
				while (++Inner.Min <= Current.Max && !Predicate(*Current.Min, *Inner.Min));
				// �ҵ���һ��С��*Current.Min����
				// Ҳ����˵Innner.Max֮���ȫ������*Current.Min
				while (--Inner.Max > Current.Min && !Predicate(*Inner.Max, *Current.Min));
				if (Inner.Min > Inner.Max)
				{
					break;
				}

				Exchange(*Inner.Min, *Inner.Max);
			}

			// ��Current.Min�������м�ȥ
			// ��ˣ����ڵ������
			// ---------1----------|------2------|-------3-----------------
			//					Inner.Max     Inner.Min
			// 2�����ֵ��Ϊ�ֽ��ߵ�ֵ��1�����ֵȫ��С�ڵ��ڷֽ���
			// 3�����ֵȫ�����ڵ��ڷֽ��ߵ�ֵ
			Exchange(*Current.Min, *Inner.Max);

			// ��ʱInner.Max��ָ��Ԫ�ص�ֵ���ڷֽ��ߵ�ֵ

			// ��Ԫ�ض��һ������ջ������Ԫ���ٵ�һ��
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