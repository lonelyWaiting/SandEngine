#pragma once

template<typename T>
struct TIsArithmetic
{
	enum
	{
		Value = false
	};
};

template<> struct TIsArithmetic<float>
{
	enum
	{
		Value = true
	};
};


template<> struct TIsArithmetic<double>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<long double>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<suInt8>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<suInt16>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<suInt32>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<suInt64>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<sInt8>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<sInt16>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<sInt32>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<sInt64>
{
	enum
	{
		Value = true
	};
};

template<> struct TIsArithmetic<bool>
{
	enum
	{
		Value = true
	};
};

template<typename T> struct TIsArithmetic<const T>
{
	enum
	{
		Value = TIsArithmetic<T>::Value
	};
};

template<typename T> struct TIsArithmetic<volatile T>
{
	enum
	{
		Value = TIsArithmetic<T>::Value
	};
};

template<typename T> struct TIsArithmetic<const volatile T>
{
	enum
	{
		Value = TIsArithmetic<T>::Value
	};
};