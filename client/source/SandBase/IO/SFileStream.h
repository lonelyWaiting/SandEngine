#pragma once

#include "SandBase/Vector/SArray.h"

class SFileStream
{
public:
	void BeginChunk( long pos );
	long EndChunk();

protected:
	SArray<long> chunkPos;
};

#include <fstream>

class SVector2f;
class SVector3f;
class SVector4f;

class SFileInStream : public SFileStream
{
public:
	bool OpenFile( const char* szFilename );
	void Close();

	suInt32 OpenChunk();
	void	CloseChunk();

	bool ReadInt( int& value );
	bool ReadFloat( float& value );
	bool ReadString( char* str );
	bool Read( char* ptr , int sizeInByte );
	bool Read( SVector2f& v );
	bool Read( SVector3f& v );
	bool Read( SVector4f& v );

	int GetFileSize();

public:
	std::ifstream ifs;
};

class SFileOutStream : public SFileStream
{
public:
	bool OpenFile( const char* filename );
	void Close();

	bool BeginChunk( suInt32 chunkName );
	void EndChunk();

	void SetCurPos( long pos );
	long GetCurPos();

	void WriteInt( const int value );
	void WriteLong( const long value );
	void WriteUInt32( const suInt32 value );
	void WriteFloat( const float value );
	void WriteString( const char* str );
	void WriteVector( const SVector2f& v );
	void WriteVector( const SVector3f& v );
	void WriteVector( const SVector4f& v );
	void Write( const char* ptr , int SizeInBytes );

protected:
	std::ofstream ofs;
};

typedef void (*EnumCallback)(const char* szDir, const char* filename, void* userData);
namespace FileSystem
{
	void AddSearchPath(const char* path);
	void EnumFile(const char* folder, const char* pattern, EnumCallback callback, void* userData);
}