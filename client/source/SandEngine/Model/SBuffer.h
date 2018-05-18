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

enum eBufferUsage
{
	eBU_Default  ,	// gpu:read+write
	eBU_Static,		// gpu:read
	eBU_Dynamic  ,	// gpu:read,cpu:write
	eBU_Staging		// support data transfer copy from the gpu to the cpu
};

enum eBufferType
{
	eBT_Vertex ,
	eBT_Index  ,
	eBT_Constant
};

enum eBufferBindFlag
{
	eBBF_None,
	eBBF_SRV ,
	eBBF_UAV ,
	eBBF_StreamOut
};

#include "SResource.h"

struct ID3D11ShaderResourceView;
struct ID3D11UnorederAccessView;
struct ID3D11Buffer;

class SBuffer : public SRefCounter
{
public:
	SBuffer( eBufferUsage usage , eBufferType type , int stride , const void* pInitData = nullptr , eBufferBindFlag bindFlag = eBBF_None , int miscFlag = 0 );
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