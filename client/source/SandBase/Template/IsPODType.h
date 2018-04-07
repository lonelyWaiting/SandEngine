#pragma once

#include "AndOrNot.h"
#include "TIsArithmetic.h"
#include "TIsPointer.h"

template<typename T>
struct TIsPODType
{
	enum
	{
		Value = TOrValue<__is_pod( T ) || __is_enum( T ) , TIsArithmetic<T> , TIsPointer<T>>::Value
	};
};