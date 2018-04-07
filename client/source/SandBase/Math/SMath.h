#pragma once

namespace SMath
{
	template<typename T>
	T Min(const T& A, const T& B)
	{
		return A < B ? A : B;
	}

	template<typename T>
	T Max(const T& A, const T& B)
	{
		return A > B ? A : B;
	}

	template<typename T>
	void clamp( T& value , const T& a , const T& b )
	{
		T _min = a;
		T _max = b;

		if( _min > _max ) { T& temp = _max; _max = _min; _min = temp; }

		if( value > _max )	value = _max;
		if( value < _min )	value = _min;
	}

	template<typename T>
	void swap( T& A , T& B )
	{
		T temp = A;
		A = B;
		B = temp;
	}
};