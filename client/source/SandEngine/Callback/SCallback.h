#pragma once

#include "SandEngine/SApiDef.h"

struct SCallbackUserData
{
	void* pSender   = nullptr;
	void* pUserData = nullptr;
};

class SCallbackHandle
{
public:
	SAND_API virtual void OnHandle( const SCallbackUserData& userData ) = 0;
};

#include "SandBase/Vector/SArray.h"

class SCallback
{
public:
	SAND_API void operator+=( SCallbackHandle& handle );
	SAND_API void operator-=( SCallbackHandle& handle );

	void Trigger( SCallbackUserData* data = nullptr );

protected:
	SArray<SCallbackHandle*> handles;
};

class SCallbackManager
{
public:
	SCallback OnEngineInit;
	SCallback OnEngineDeInit;
	SCallback OnBeginRender;
	SCallback OnEndRender;
	SCallback OnUpdateScene;
	SCallback OnResize;
	SCallback OnDropFile;
	SCallback OnMouseDown;
	SCallback OnMouseUp;
	SCallback OnMouseMove;
	SCallback OnKeyPressed;
	SCallback OnKeyRelease;
};