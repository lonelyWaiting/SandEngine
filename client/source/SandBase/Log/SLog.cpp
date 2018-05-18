#include <SandBase/SandBasePCH.h>
#include <iostream>
#include <windows.h>
#include "SLog.h"

template<int size>
inline void process( const char * format , va_list args , char( &errorBuf )[size] )
{
#ifdef _UNICODE
	_vsnwprintf_s( errorBuf , size , _TRUNCATE , format , args );
#else
	_vsnprintf_s( errorBuf , size , _TRUNCATE , format , args );
#endif
}

void SLog::Error( const char * format , ... )
{
	char errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , errorBuf );
	va_end( args );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ) , FOREGROUND_RED );
	std::cout << errorBuf << std::endl;
}

void SLog::Warning( const char * format , ... )
{
	char warningBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , warningBuf );
	va_end( args );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ) , FOREGROUND_RED | FOREGROUND_GREEN );
	std::cout << warningBuf << std::endl;
}

void SLog::Info( const char * format , ... )
{
	char infoBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , infoBuf );
	va_end( args );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ) , FOREGROUND_GREEN );
	std::cout << infoBuf << std::endl;
}