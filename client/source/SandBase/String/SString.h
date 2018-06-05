#pragma once

#include "SandBase/Vector/SArray.h"

class SString
{
public:
	SString();
	SString( const char* str );
	SString( const SString& str );

	SString& operator=( const char* src );
	SString& operator=( const SString& str );

	void Append( const char* str );
	void AppendFormat( const char* format , ... );
	
	bool StartWith_NoCase( const char* str );
	bool StartWith( const char* str );
	bool EndWith_NoCase( const char* str );
	bool EndWith( const char* str );

	void toLower();
	void toUpper();

	int GetLength() const;
	bool IsEmpty() const;

	int Find(const char* str) const;
	int Find_NoCase( const char* str ) const;

	void Split( SArray<SString>& tokens , const char* separator );
	
	const char* AsChar() const;
	bool IsEqual( const char* str ) const;

private:
	bool StartWith_Internal( const char* str , bool( *cmp_function )( const char , const char ) );
	bool EndWith_Internal( const char* str , bool( *cmp_function )( const char , const char ) );
	int Find_Internal( const char* str , bool( *cmp_function )( const char , const char ) , int startSearch = 0 ) const;

protected:
	SArray<char> content;
};