#pragma once

class SConstantBuffer : public SBuffer
{
public:
	SConstantBuffer( bool dynamic , int stride , const void* pInitData = nullptr );
};