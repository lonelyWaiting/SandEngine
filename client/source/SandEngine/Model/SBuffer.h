#pragma once

#include "SandEngine/SandEnginePCH.h"
#include "SandBase/Object/SRefCounter.h"

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

enum eMemUsage
{
	eBU_Static		    = SBIT(0),		// gpu:read
	eBU_Dynamic         = SBIT(1),		// gpu:read,cpu:write
	eBU_UAV_ByteAddress = SBIT(2),
	eBU_StructureBuffer = SBIT(3),
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
	eBF_RenderTarget = SBIT( 6 ) ,
	eBF_DepthStencil = SBIT( 7 ) ,
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