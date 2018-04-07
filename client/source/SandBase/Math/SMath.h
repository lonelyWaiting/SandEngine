#pragma once

namespace SMath
{
	template<typename T>
	T min(const T& A, const T& B)
	{
		return A < B ? A : B;
	}

	template<typename T>
	T max(const T& A, const T& B)
	{
		return A > B ? A : B;
	}

	template<typename T>
	void clamp( T& value , const T& a , const T& b )
	{
		T min = a;
		T max = b;

		if( min > max ) { T& temp = max; max = min; min = temp; }

		if( value > max )	value = max;
		if( value < min )	value = min;
	}

	template<typename T>
	void swap( T& A , T& B )
	{
		T temp = A;
		A = B;
		B = temp;
	}
};