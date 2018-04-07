#pragma once

#include "AndOrNot.h"
#include "IsPODType.h"

template<typename T>
struct TIsTriviallyCopyConstructible
{
	enum
	{
		Value = TOrValue<__has_trivial_copy(T), TIsPODType<T>>::Value
	};
};