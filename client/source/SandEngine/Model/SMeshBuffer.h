//#pragma once
#ifndef SMESHBUFFER_H
#define SMESHBUFFER_H

#include "SVertexBuffer.h"
#include "SIndexBuffer.h"
#include "SandBase/Object/SSmartPointer.h"
#include "SandBase/Object/SRefCounter.h"

class SMeshBuffer : public SRefCounter
{
public:
	void EnsureVertexBuffer( const SVertexDescription& desc , eBufferUsage usage , int iNumOfVertices , const void* pVertices = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );
	void EnsureIndexBuffer( eBufferUsage usage , int iNumOfIndices , const void* pIndices = nullptr , eIndexFormat format = eIF_Short , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );

	SVertexBuffer* GetVertexBuffer();
	SIndexBuffer*  GetIndexBuffer();

	int GetNumOfVertices();
	int GetNumOfIndices();

	void SetFilename( const char* filename );
private:
	SVertexBufferPtr m_VertexBuffer = nullptr;
	SIndexBufferPtr  m_IndexBuffer  = nullptr;
};

typedef SSmartPointer<SMeshBuffer> SMeshBufferPtr;
#endif