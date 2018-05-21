#pragma once
#ifndef STRICT
#define STRICT
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

// Standard C++ Library
#include <fstream>
#include <string.h>
#include <map>
#include <list>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
#include <iostream>

// Standard C Library
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <assert.h>
#include <fcntl.h>
#include <io.h>

// DirectX Header
#include <d3d11.h>
#include <d3dCompiler.h>

#include <wrl.h>
#include <windows.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release(); (x) = nullptr;}}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete (x); (x) = nullptr;}}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[] x; (x) = nullptr;}}
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	{if(x){free((x)); (x) = nullptr;}}
#endif

#ifndef SAND_PI
#define SAND_PI 3.14159265f
#endif

#ifndef ConvertToRadius
#define ConvertToRadius(fDegree)	(fDegree) * (SAND_PI / 180.0f)
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)	((int)(short)((WORD)(((DWORD_PTR)(lp)) & 0xffff)))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)	((int)(short)((WORD)((((DWORD_PTR)(lp)) >> 16) & 0xffff)))
#endif

#ifndef suInt32
#define suInt32	unsigned int
#endif

#ifndef sInt32
#define sInt32	int
#endif

#ifndef suInt8
#define suInt8	unsigned char
#endif

#ifndef sInt8
#define sInt8	char
#endif

#ifndef suInt16
#define suInt16	unsigned short
#endif

#ifndef sInt16
#define sInt16	short
#endif

#ifndef suInt64
#define suInt64	unsigned __int64
#endif

#ifndef sInt64
#define sInt64	__int64
#endif

#ifndef Float
#define Float	float
#endif

#define SBIT(n)			(1 << n)

enum EInputMask
{
	EIM_NONE      = SBIT( 0 ) ,
	EIM_POSITION  = SBIT( 1 ) ,
	EIM_NORMAL    = SBIT( 2 ) ,
	EIM_TANGENT   = SBIT( 3 ) ,
	EIM_WEIGHT    = SBIT( 4 ) ,
	EIM_TEXCOORD0 = SBIT( 5 ) ,
	EIM_TEXCOORD1 = SBIT( 6 ) ,
	EIM_TEXCOORD2 = SBIT( 7 ) ,
	EIM_TEXCOORD3 = SBIT( 8 ) ,
	EIM_TEXCOORD4 = SBIT( 9 ) ,
	EIM_TEXCOORD5 = SBIT( 10 ) ,
	EIM_TEXCOORD6 = SBIT( 11 ) ,
	EIM_TEXCOORD7 = SBIT( 12 ) ,
	EIM_COLOR     = SBIT( 13 )
};

enum EMapType
{
	EMT_DIFFUSE ,
	EMT_NORMAL ,
	EMT_SPECULAR ,
	EMT_METAILLC ,
	EMT_ROUGHNESS ,
	EMT_EMISSIVE ,
	EMT_MAPNUM
};

#define EPSILON 10e-5f
#define INDEX_NONE -1

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif
typedef wchar_t WIDECHAR;
typedef char	ANSICHAR;
// A 16-bit character containing a UCS2 (Unicode, 16-bit, fixed-width) code unit, used for compatibility with 'Windows TCHAR' across multiple platforms.
typedef unsigned short UCS2CHAR;

typedef signed long long PTRINT;

#ifndef Max_int8
#define Max_int8	((int8)		0x7f)
#endif

#ifndef Max_int16
#define Max_int16	((int16)	0x7fff)
#endif

#ifndef Max_int32
#define Max_int32	((int32)	0x7fffffff)
#endif

#ifndef Max_int64
#define Max_int64	((int64)	0x7fffffffffffffff)
#endif

#ifndef Max_uint8
#define Max_uint8	((uint8)	0xff)
#endif

#ifndef Max_uint16
#define Max_uint16	((uint16)	0xffff)
#endif

#ifndef Max_uint32
#define Max_uint32	((uint32)	0xffffffff)
#endif

#ifndef Max_uint64
#define Max_uint64	((uint64)	0xffffffffffffffff)
#endif

#ifndef RESTRICT
#define RESTRICT	__restrict
#endif

#define Enum2Str(str)	#str