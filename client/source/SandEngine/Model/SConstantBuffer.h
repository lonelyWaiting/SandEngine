#pragma once

class SConstantBuffer : public SBuffer
{
public:
	SConstantBuffer( int stride , const void* pInitData = nullptr );
};