#pragma once

template<typename T = void>
struct TGreater
{
	inline bool operator()( const T& A , const T& B ) const
	{
		return B < A;
	}
};

template<>
struct TGreater<void>
{
	template<typename T>
	inline bool operator()( const T& A , const T& B ) const
	{
		return B < A;
	}
};