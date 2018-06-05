#pragma once

#include "SandEngine/SandEnginePCH.h"
#include "SandBase/Object/SRefCounter.h"
#include "SandBase/Object/SSmartPointer.h"

// directx resouces limit:https://msdn.microsoft.com/en-us/library/windows/desktop/ff819065(v=vs.85).aspx
// in directx 11 , input assembler vertex input resouces slots is 32
enum eVertexFormat
{
	eVF_Float3 = SBIT( 16 ) ,
	eVF_Float4 = SBIT( 17 ) ,
	eVF_Float2 = SBIT( 18 ) ,
	eVF_Float  = SBIT( 19 ) ,
	eVF_UByte4 = SBIT( 20 )
};

enum eVertexAttribute
{
	eVA_None        = 0 ,
	eVA_POSITION    = SBIT( 0 ) ,
	eVA_NORMAL      = SBIT( 1 ) ,
	eVA_VERTEXCOLOR = SBIT( 2 ) ,
	eVA_TEXCOORD0   = SBIT( 3 ) ,
	eVA_TEXCOORD1   = SBIT( 4 ) ,
	eVA_TEXCOORD2   = SBIT( 5 ) ,
	eVA_TEXCOORD3   = SBIT( 6 ) ,
	eVA_TEXCOORD4   = SBIT( 7 ) ,
	eVA_TEXCOORD5   = SBIT( 8 ) ,
	eVA_TEXCOORD6   = SBIT( 9 ) ,
	eVA_TEXCOORD7   = SBIT( 10 ) ,
	eVA_TEXCOORD8   = SBIT( 11 ) ,
	eVA_TEXCOORD9   = SBIT( 12 ) ,
	eVA_TEXCOORD10  = SBIT( 13 ) ,
	eVA_TEXCOORD11  = SBIT( 14 ) ,
	eVA_TEXCOORD12  = SBIT( 15 ) ,
	eVA_TEXCOORD13  = SBIT( 16 ) ,
	eVA_TEXCOORD14  = SBIT( 17 ) ,
	eVA_TEXCOORD15  = SBIT( 18 ) ,
};

enum eMemUsage
{
	eBU_Static          = SBIT( 0 ) ,		// gpu:read
	eBU_Dynamic         = SBIT( 1 ) ,		// gpu:read,cpu:write
	eBU_UAV_ByteAddress = SBIT( 2 ) ,
	eBU_StructureBuffer = SBIT( 3 ) ,
	eBU_UAV_Append      = SBIT( 4 ) ,
	eBU_UAV_Count       = SBIT( 5 ),
};

enum eBindFlag
{
	eBF_None         = 0 ,
	eBF_Vertex       = SBIT( 0 ) ,
	eBF_Index        = SBIT( 1 ) ,
	eBF_Constant     = SBIT( 2 ) ,
	eBF_SRV          = SBIT( 3 ) ,
	eBF_UAV          = SBIT( 4 ) ,
	eBF_StreamOut    = SBIT( 5 ) ,
};

#include "SResource.h"

struct ID3D11ShaderResourceView;
struct ID3D11UnorederAccessView;
struct ID3D11Buffer;

class SBuffer : public SRefCounter
{
public:
	SBuffer( eMemUsage usage , int stride , int count , const void* pInitData = nullptr , eBindFlag bindFlag = eBF_None );
	~SBuffer();

	void SetFilename( const char* filename );

	ID3D11ShaderResourceView*  GetShaderResourceView()  const;
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

	void* Lock();
	void  UnLock();

protected:
	ID3D11Buffer *             m_pBuffer  = nullptr;
	ID3D11ShaderResourceView*  m_pSRV     = nullptr;
	ID3D11UnorderedAccessView* m_pUAV     = nullptr;
	bool                       m_Lockable = false;
};

struct SVertexDescription
{
	int m_iPos          = -1;
	int m_iNormal       = -1;
	int m_iTexcoord[16] = { -1 };
	int m_color         = -1;
	int stride          = 0;
};

class SVertexBuffer : public SBuffer
{
public:
	SVertexBuffer( const SVertexDescription& desc , eMemUsage usage , int iNumOfVertices = 0 , const void* pInitData = nullptr , eBindFlag viewFlag = eBF_None );
	int GetVertexMask();
	int GetNumOfVertices();

private:
	SVertexDescription m_vertexDesc;
	int m_iNumOfVertices;
};

typedef SSmartPointer<SVertexBuffer> SVertexBufferPtr;

enum eIndexFormat
{
	eIF_Short ,
	eIF_Int
};

class SIndexBuffer : public SBuffer
{
public:
	SIndexBuffer( eMemUsage usage , int iNumOfIndices = 0 , eIndexFormat format = eIF_Short , const void* pInitData = nullptr , eBindFlag bindFlag = eBF_None );
	eIndexFormat GetIndexType();
	int GetNumOfIndices();

private:
	eIndexFormat m_IndexType = eIF_Short;
	int m_iNumOfIndices = 0;
};

typedef SSmartPointer<SIndexBuffer> SIndexBufferPtr;