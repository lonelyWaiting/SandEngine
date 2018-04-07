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

#ifndef int64
#define int64	signed long long
#endif

#ifndef uint64
#define uint64	unsigned long long
#endif

#ifndef int32
#define int32	int
#endif

#ifndef uint32
#define uint32	unsigned int
#endif

#ifndef int16
#define int16	signed short
#endif

#ifndef uint16
#define uint16	unsigned short
#endif

#ifndef int8
#define int8	signed char
#endif

#ifndef uint8
#define uint8	unsigned char
#endif

#ifndef Float
#define Float	float
#endif

#ifndef ushort
#define ushort	unsigned short int
#endif

#define VBit(i)	1 << i
enum EInputMask
{
	EIM_NONE = VBit( 0 ) ,
	EIM_POSITION = VBit( 1 ) ,
	EIM_NORMAL = VBit( 2 ) ,
	EIM_TANGENT = VBit( 3 ) ,
	EIM_WEIGHT = VBit( 4 ) ,
	EIM_TEXCOORD0 = VBit( 5 ) ,
	EIM_TEXCOORD1 = VBit( 6 ) ,
	EIM_TEXCOORD2 = VBit( 7 ) ,
	EIM_TEXCOORD3 = VBit( 8 ) ,
	EIM_TEXCOORD4 = VBit( 9 ) ,
	EIM_TEXCOORD5 = VBit( 10 ) ,
	EIM_TEXCOORD6 = VBit( 11 ) ,
	EIM_TEXCOORD7 = VBit( 12 ) ,
	EIM_COLOR = VBit( 13 )
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

enum { INDEX_NONE = -1 };

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