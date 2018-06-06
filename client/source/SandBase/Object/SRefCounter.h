#pragma once

class SRefCounter
{
public:
	void AddRef();
	void Release();
	virtual ~SRefCounter() {}

	int GetRefCount();

private:
	int m_refCount = 0;
};