#pragma once

#include "SandEngine/SApiDef.h"

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
	void Render();

private:
	SWindow * m_pWindow;
};