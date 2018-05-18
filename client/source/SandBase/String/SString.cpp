#include "SandBase/SandBasePCH.h"
#include "SString.h"
#include <cstring>
#include <cstdarg>
#include <cstdio>

SString::SString()
{
	content.PushBack( '\0' );
}

SString::SString( const char * str )
{
	if( str )
	{
		int len = ( int )strlen( str );
		content.Reserve( len + 1 );

		for( int i = 0; i < len; i++ )
		{
			content.PushBack( str[i] );
		}
	}

	content.PushBack( '\0' );
}

SString::SString( const SString & str )
{
	int len = str.GetLength();
	content.Resize( len + 1 );
	for( int i = 0; i < len; i++ )
	{
		content[i] = str.AsChar()[i];
	}

	content[len] = '\0';
}

SString & SString::operator=( const char * src )
{
	int len = ( int )strlen( src );
	content.Resize( len + 1 );
	for( int i = 0; i <= len; i++ )	content[i] = src[i];

	return *this;
}

SString & SString::operator=( const SString & str )
{
	*this = str.AsChar();

	return *this;
}

void SString::Append( const char * str )
{
	unsigned int len = ( unsigned int )strlen( str );

	content.Reserve( GetLength() + len + 1 );
	content.Insert( str , len , GetLength() );
}

void SString::AppendFormat( const char * format , ... )
{
	va_list args;

	char buf[1024];

	va_start( args , format );
	vsprintf_s( buf , format , args );
	va_end( args );

	unsigned int len = ( unsigned int )strlen( buf );

	content.Reserve( GetLength() + len + 1 );
	content.Insert( buf , len , GetLength() );
}

bool SString::StartWith_NoCase( const char * str )
{
	return StartWith_Internal( str , []( const char src , const char dest )
	{
		if( src == dest )
			return true;

		if( src >= 'A' && src <= 'Z' && dest == src + 32 )
		{
			return true;
		}
		else if( src >= 'a' && src <= 'z' && dest == src - 32 )
		{
			return true;
		}

		return false;
	} );
}

bool SString::StartWith( const char * str )
{
	return StartWith_Internal( str , []( const char str , const char dest )
	{
		return str == dest;
	} );
}

bool SString::EndWith_NoCase( const char * str )
{
	return EndWith_Internal( str , []( const char src , const char dest )
	{
		if( src == dest )
			return true;

		if( src >= 'A' && src <= 'Z' && dest == src + 32 )
		{
			return true;
		}
		else if( src >= 'a' && src <= 'z' && dest == src - 32 )
		{
			return true;
		}

		return false;
	} );
}

bool SString::EndWith( const char * str )
{
	return EndWith_Internal( str , []( const char src , const char dest )
	{
		return src == dest;
	} );
}

void SString::toLower()
{
	for( int i = 0 , len = GetLength(); i < len; i++ )
	{
		if( content[i] >= 'A' && content[i] <= 'Z' )
		{
			content[i] += 32;
		}
	}
}

void SString::toUpper()
{
	for( int i = 0 , len = GetLength(); i < len; i++ )
	{
		if( content[i] >= 'a' && content[i] <= 'z' )
		{
			content[i] -= 32;
		}
	}
}

int SString::GetLength() const
{
	return content.GetSize() - 1;
}

bool SString::IsEmpty() const
{
	return content.GetSize() == 1;
}

int SString::Find( const char * str ) const
{
	return Find_Internal(str , []( const char src , const char dest )
	{
		return src == dest;
	} );
}

int SString::Find_NoCase( const char * str ) const
{
	return Find_Internal( str , []( const char src , const char dest )
	{
		if( src == dest )
			return true;

		if( src >= 'A' && src <= 'Z' && dest == src + 32 )
		{
			return true;
		}
		else if( src >= 'a' && src <= 'z' && dest == src - 32 )
		{
			return true;
		}

		return false;
	} );
}

void SString::Split( SArray<SString>& tokens , const char * separator )
{
	if( !separator )	return;

	tokens.Clear();

	SArray<int> splitIdx;

	int idx = -1 , len = ( int )strlen( separator );
	if( len == 0 )	return;

	do
	{
		idx = Find_Internal( separator,
							 []( const char src , const char dest )
							 {
								 return src == dest;
							 } , 
							 idx == -1 ? 0 : idx + len );

		if( idx != -1 )	splitIdx.PushBack( idx );

	} while( idx != -1 );

	int count = splitIdx.GetSize();
	if( count == 0 )	return;

	tokens.Reserve( count );

	for( int i = 0 , start = 0; i < count + 1; i++ )
	{
		int end = i < count ? splitIdx[i] : GetLength();

		if( int size = end - start )
		{
			tokens.PushBack( SString() );
			auto& token = tokens.Peek();
			token.content.Reserve( size );
			token.content.Insert( AsChar() + start , size , 0 );
		}

		start = end + len;
	}
}

const char * SString::AsChar() const
{
	return content.GetData();
}

bool SString::StartWith_Internal( const char * str , bool( *cmp_function )( const char , const char ) )
{
	bool result = true;

	int len = ( int )strlen( str );

	if( len > GetLength() )
	{
		result = false;
	}
	else
	{
		for( int i = 0; i < len; i++ )
		{
			if( !cmp_function( *( str + i ) , content[i] ) )
			{
				result = false;
				break;
			}
		}
	}

	return result;
}

bool SString::EndWith_Internal( const char * str , bool( *cmp_function )( const char , const char ) )
{
	bool result = true;

	int len = ( int )strlen( str );
	int dest_len = GetLength();

	if( len > dest_len )
	{
		result = false;
	}
	else
	{
		for( int i = 0; i < len; i++ )
		{
			if( !cmp_function( *( str + i ) , content[dest_len - len + i] ) )
			{
				result = false;
				break;
			}
		}
	}
	return result;
}

int SString::Find_Internal( const char * str , bool( *cmp_function )( const char , const char ) , int startSearch /*= 0*/ ) const
{
	auto get_next = [cmp_function]( const char* str , SArray<int>& next )
	{
		if( !str )	return;
		int len = ( int )strlen( str );
		if( len == 0 )	return;
		next.Resize( len );
		next[0] = -1;
		int k = -1 , j = 0;
		while( j < len - 1 )
		{
			if( k == -1 || cmp_function( str[j] , str[k] ) )
			{
				++k;
				++j;
				next[j] = k;
			}
			else
			{
				k = next[k];
			}
		}
	};

	SArray<int> next;
	get_next( str , next );

	int i = startSearch;
	int j = 0;
	int srcLen = GetLength();
	int matchLen = ( int )strlen( str );

	while( i < srcLen && j < matchLen )
	{
		//①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++      
		if( j == -1 || cmp_function( content[i] , str[j] ) )
		{
			i++;
			j++;
		}
		else
		{
			//②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]      
			//next[j]即为j所对应的next值        
			j = next[j];
		}
	}

	return j == matchLen ? i - j : -1;
}