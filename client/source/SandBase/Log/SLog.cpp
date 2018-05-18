#include <SandBase/SandBasePCH.h>
#include <stdarg.h>
#include <iostream>
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

#define RESET	"\x1B[0m"
#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"

void SLog::Error( const char * format , ... )
{
	char errorBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , errorBuf );
	va_end( args );

	std::cout << RED << errorBuf << RESET << std::endl;
}

void SLog::Warning( const char * format , ... )
{
	char warningBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , warningBuf );
	va_end( args );

	std::cout << YELLOW << warningBuf << RESET << std::endl;
}

void SLog::Info( const char * format , ... )
{
	char infoBuf[2048];
	va_list args;
	va_start( args , format );
	process( format , args , infoBuf );
	va_end( args );

	std::cout << GREEN << infoBuf << RESET << std::endl;
}