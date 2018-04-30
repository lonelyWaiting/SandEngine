#include "SandEnginePCH.h"
#include "SBuffer.h"
#include "SConstantBuffer.h"

SConstantBuffer::SConstantBuffer( int stride , const void * pInitData /*= nullptr*/ )
	:SBuffer( eBU_Dynamic , eBT_Constant , stride , pInitData , eBBF_None )
{

}