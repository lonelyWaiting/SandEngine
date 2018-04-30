#pragma once

class SRefCounter
{
public:
	void Increase();
	void Release();

	int GetRefCount();

private:
	int m_refCount = 0;
};