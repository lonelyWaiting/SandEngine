#pragma once

#include "SandEngine/SApiDef.h"

class SCallbackManager;

class SandEngine
{
public:
	SAND_API static SCallbackManager	Callback;
};

void SandEngineInit();
void SandEngineDeInit();