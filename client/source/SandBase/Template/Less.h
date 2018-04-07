#pragma once

// Binary predicate class for sorting elements in order.
// Assumes < operator is defined for the template type.
// see: http://en.cppreference.com/w/cpp/utility/functional/less
template<typename T = void>
struct TLess
{
	inline bool operator()( const T& A , const T& B ) const
	{
		return A < B;
	}
};

template<>
struct TLess<void>
{
	template<typename T>
	inline bool operator()( const T& A , const T& B ) const
	{
		return A < B;
	}
};