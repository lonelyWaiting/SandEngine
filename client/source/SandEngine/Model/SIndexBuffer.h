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
	SIndexBuffer( eMemUsage usage , int iNumOfIndices = 0, eIndexFormat format = eIF_Short , const void* pInitData = nullptr , eBindFlag bindFlag = eBF_None );
	eIndexFormat GetIndexType();
	int GetNumOfIndices();

private:
	eIndexFormat m_IndexType = eIF_Short;
	int m_iNumOfIndices = 0;
};

typedef SSmartPointer<SIndexBuffer> SIndexBufferPtr;
#endif