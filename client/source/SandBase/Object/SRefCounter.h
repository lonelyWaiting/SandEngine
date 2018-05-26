#pragma once

class SRefCounter
{
public:
	void AddRef();
	void Release();

	int GetRefCount();

private:
	int m_refCount = 0;
};