#pragma once

template<typename T>
struct TIsTriviallyDestructible
{
	enum
	{
		Value = __has_trivial_destructor(T)
	};
};