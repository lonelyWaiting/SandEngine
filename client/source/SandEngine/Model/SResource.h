#pragma once

#include "SandBase/Object/SRefCounter.h"
#include "SandBase/String/SString.h"

enum eResourceFlag
{
	eRF_None    = 0 ,
	eRF_Loading = SBIT( 0 ) ,
	eRF_Loaded  = SBIT( 1 )
};

class SResource : public SRefCounter
{
public:
	SResource( const char* name = nullptr );
	const SString& GetFilename();
	void SetFileName( const char* name );
	void SetResourceFlag( eResourceFlag flag );
	int  GetResourceFlag() const;
	bool HasFlag( eResourceFlag& flag ) const;

private:
	SString m_name;
	int m_resourceFlag = eRF_None;
};