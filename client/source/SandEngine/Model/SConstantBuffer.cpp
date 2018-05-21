#include "SandEnginePCH.h"
#include "SBuffer.h"
#include "SConstantBuffer.h"

SConstantBuffer::SConstantBuffer( bool dynamic , int stride , const void * pInitData /*= nullptr*/ )
	:SBuffer( dynamic ? eBU_Dynamic : eBU_Static , stride , 1 , pInitData , eBF_Constant )
{

}