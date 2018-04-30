#pragma once

#include "SBuffer.h"
#include "SandBase/Object/SSmartPointer.h"

enum eIndexType
{
	eIT_Short ,
	eIT_Int
};

class SIndexBuffer : public SBuffer
{
public:
	SIndexBuffer( eBufferUsage usage , int stride , const void* pInitData = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );

	void SetIndexType( eIndexType type );
	eIndexType GetIndexType();

private:
	eIndexType m_IndexType = eIT_Short;
};

typedef SSmartPointer<SIndexBuffer> SIndexBufferPtr;