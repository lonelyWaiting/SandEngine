//#pragma once
#ifndef SMESHBUFFER_H
#define SMESHBUFFER_H

#include "SVertexBuffer.h"
#include "SIndexBuffer.h"
#include "SandBase/Object/SSmartPointer.h"
#include "SandBase/Object/SRefCounter.h"
#include "SandEngine/Application/RenderState/RenderState.h"

class SMeshBuffer : public SRefCounter
{
public:
	void EnsureVertexBuffer( const SVertexDescription& desc , eMemUsage usage , int iNumOfVertices , const void* pVertices = nullptr , eBindFlag bindFlag = eBF_None );
	void EnsureIndexBuffer( eMemUsage usage , int iNumOfIndices , const void* pIndices = nullptr , eIndexFormat format = eIF_Short , eBindFlag bindFlag = eBF_None );

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