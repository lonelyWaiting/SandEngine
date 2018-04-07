#pragma once

class SWindow;

class SApplication
{
public:
	SApplication();
	~SApplication();

public:
	bool Init();
	void Run();

private:
	void Shutdown();
	void Update();

private:
	SWindow * m_pWindow;
};