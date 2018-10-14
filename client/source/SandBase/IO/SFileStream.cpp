#include "SandBase/SandBasePCH.h"
#include "SFileStream.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SVector4f.h"
#include <filesystem>

void SFileStream::BeginChunk( long pos )
{
	chunkPos.PushBack( pos );
}

long SFileStream::EndChunk()
{
	return chunkPos.Pop();
}

bool SFileInStream::OpenFile( const char* szFilename )
{
	ifs.open( szFilename , std::ios::in | std::ios::binary );
	return ifs.is_open();
}

bool SFileInStream::Read( char* ptr , int sizeInByte )
{
	ifs.read((char*)ptr, sizeInByte);
	return ifs ? true : false;
}

bool SFileInStream::Read(SVector2f & v)
{
	ifs.read((char*)v.m, sizeof(float) * 2);
	return ifs ? true : false;
}

bool SFileInStream::Read(SVector3f & v)
{
	ifs.read((char*)v.m, sizeof(float) * 3);
	return ifs ? true : false;
}

bool SFileInStream::Read(SVector4f & v)
{
	ifs.read((char*)v.m, sizeof(float) * 4);
	return ifs ? true : false;
}

bool SFileInStream::ReadInt( int& value )
{
	ifs.read((char*)(&value), sizeof(int));
	return ifs ? true : false;
}

bool SFileInStream::ReadFloat( float& value )
{
	ifs.read((char*)(&value), sizeof(float));
	return ifs ? true : false;
}

bool SFileInStream::ReadString( char* str )
{
	int size = 0;
	if (!ReadInt(size))	return false;

	ifs.read(str, size);
	return ifs ? true : false;
}

void SFileInStream::Close()
{
	ifs.close();
}

int SFileInStream::GetFileSize()
{
	int curPos = (int)ifs.tellg();
	ifs.seekg( 0 , std::ios::end );
	int size = (int)ifs.tellg();
	ifs.seekg( curPos );
	return size;
}

suInt32 SFileInStream::OpenChunk()
{
	int ChunkID;
	if( ReadInt( ChunkID ) )
	{
		chunkPos.PushBack( ChunkID );
		return true;
	}
	return false;
}

void SFileInStream::CloseChunk()
{
	chunkPos.Pop();
}

bool SFileOutStream::OpenFile( const char* filename )
{
	ofs.open( filename , std::ios::out | std::ios::binary );
	return ofs.is_open();
}

void SFileOutStream::WriteLong( const long value )
{
	ofs.write( ( const char* )( &value ) , sizeof( value ) );
}

void SFileOutStream::WriteInt( const int value )
{
	ofs.write( ( const char* )( &value ) , sizeof( value ) );
}

void SFileOutStream::WriteUInt32( const suInt32 value )
{
	ofs.write( ( const char* )( &value ) , sizeof( value ) );
}

void SFileOutStream::WriteFloat( const float value )
{
	ofs.write( ( const char* )( &value ) , sizeof( value ) );
}

void SFileOutStream::WriteString( const char* str )
{
	int SizeInBytes = ( int )strlen( str );
	WriteInt( SizeInBytes );
	ofs.write( str , SizeInBytes );
}

void SFileOutStream::WriteVector( const SVector2f& v )
{
	WriteFloat( v[0] );
	WriteFloat( v[1] );
}

void SFileOutStream::WriteVector( const SVector3f& v )
{
	WriteFloat( v[0] );
	WriteFloat( v[1] );
	WriteFloat( v[2] );
}

void SFileOutStream::WriteVector( const SVector4f& v )
{
	WriteFloat( v[0] );
	WriteFloat( v[1] );
	WriteFloat( v[2] );
	WriteFloat( v[3] );
}

void SFileOutStream::Write( const char* ptr , int SizeInBytes )
{
	ofs.write( ptr , SizeInBytes );
}

void SFileOutStream::Close()
{
	ofs.close();
}

bool SFileOutStream::BeginChunk( suInt32 chunkName )
{
	WriteUInt32( chunkName );
	long curChunkPos = GetCurPos();
	SFileStream::BeginChunk( curChunkPos );
	WriteLong( curChunkPos );

	return true;
}

void SFileOutStream::EndChunk()
{
	long curPos = GetCurPos();
	long chunkPos = SFileStream::EndChunk();
	SetCurPos( chunkPos );
	long size = curPos - chunkPos - sizeof( long );
	WriteLong( size );
	SetCurPos( curPos );
}

void SFileOutStream::SetCurPos( long pos )
{
	ofs.seekp( pos );
}

long SFileOutStream::GetCurPos()
{
	return (long)ofs.tellp();
}