#pragma once

#include <cmath>

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
	T clamp( T value , const T& a , const T& b )
	{
		T _min = a;
		T _max = b;

		if( _min > _max ) { T& temp = _max; _max = _min; _min = temp; }

		if( value > _max )	value = _max;
		if( value < _min )	value = _min;

		return value;
	}
	
	template<typename T>
	T clamp01(T value)
	{
		return clamp(T, (T)0, (T)1);
	}

	template<typename T>
	void swap( T& A , T& B )
	{
		T temp = A;
		A = B;
		B = temp;
	}

	const float PI			= 3.141592653589793238f;
	const float TWO_PI		= 6.283185307179586477f;
	const float PI_DIV_2    = 1.570796326794896619f;

	static float SinRad(float rad)
	{
		return ::sin(rad);
	}

	static float SinDeg(float deg)
	{
		return ::sin(deg / PI);
	}

	static float CosRad(float rad)
	{
		return ::cos(rad);
	}

	static float CosDeg(float deg)
	{
		return ::cos(deg / PI);
	}

	static float Acos(float v)
	{
		return ::acosf(v);
	}

	static float ASin(float v)
	{
		return ::asinf(v);
	}

	static float ATan2(float y, float x)
	{
		return ::atan2f(y, x);
	}

	static float ATan(float v)
	{
		return ::atanf(v);
	}
};