//#pragma once
#ifndef SINDEXBUFFER_H
#define SINDEXBUFFER_H

#include "SBuffer.h"
#include "SandBase/Object/SSmartPointer.h"

enum eIndexFormat
{
	eIF_Short ,
	eIF_Int
};

class SIndexBuffer : public SBuffer
{
public:
	SIndexBuffer( eBufferUsage usage , int iNumOfIndices = 0, eIndexFormat format = eIF_Short , const void* pInitData = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );
	eIndexFormat GetIndexType();
	int GetNumOfIndices();

private:
	eIndexFormat m_IndexType = eIF_Short;
	int m_iNumOfIndices = 0;
};

typedef SSmartPointer<SIndexBuffer> SIndexBufferPtr;
#endif