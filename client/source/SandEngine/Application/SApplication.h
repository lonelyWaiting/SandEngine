#pragma once

#include "SandEngineModule.h"

class SWindow;

class SAND_API SApplication
{
public:
	SApplication();

public:
	bool Init();
	void Run();

private:
	void Shutdown();
	void Update();
	void Render();

private:
	SWindow * m_pWindow;
};