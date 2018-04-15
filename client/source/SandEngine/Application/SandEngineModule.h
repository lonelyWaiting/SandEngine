#pragma once

#ifdef SAND_EXPORT
#define SAND_API __declspec(dllexport)
#else
#define SAND_API __declspec(dllimport)
#endif

class SCallbackManager;

class SandEngine
{
public:
	SAND_API static SCallbackManager Callback;
};

void SandEngineInit();
void SandEngineDeInit();