#pragma once

class SMeshBuffer;
class SIScene;

namespace SMeshLoader
{
	static SMeshBuffer* LoadMeshBuffer( const char* filename );
	static SMeshBuffer* LoadFromScene( const SIScene& scene );
};
