#pragma once

#include <d3d11.h>
#include "SandEngine/SApiDef.h"

class SAND_API SShader
{
public:
	SShader() {}
	bool Load(const char* path, const char* vs_main, const char* ps_main, const char* gs_main = nullptr, const char* hs_main = nullptr, const char* ds_main = nullptr, const char* cs_main = nullptr);
	
	ID3D11VertexShader*   GetVertexShader()   const	{ return mVertexShader  ; }
	ID3D11PixelShader*    GetPixelShader()    const	{ return mPixelShader   ; }
	ID3D11GeometryShader* GetGeometryShader() const { return mGeometryShader; }
	ID3D11HullShader*     GetHullShader()	  const {return mHullShader     ; }
	ID3D11DomainShader*   GetDomainShader()	  const { return mDomainShader  ; }
	ID3D11ComputeShader*  GetComputeShader()  const { return mComputeShader ; }

	ID3DBlob* GetVertexBlob() const { return mVertexBlob; }
protected:
	ID3DBlob* mVertexBlob   = nullptr;
	ID3DBlob* mPixelBlob    = nullptr ;
	ID3DBlob* mGeometryBlob = nullptr;
	ID3DBlob* mHullBlob     = nullptr;
	ID3DBlob* mDomainBlob   = nullptr;
	ID3DBlob* mComputeBlob  = nullptr;

	ID3D11VertexShader*		mVertexShader   = nullptr;
	ID3D11HullShader*		mHullShader     = nullptr;
	ID3D11DomainShader*		mDomainShader   = nullptr;
	ID3D11GeometryShader*	mGeometryShader = nullptr;
	ID3D11PixelShader*		mPixelShader    = nullptr;
	ID3D11ComputeShader*	mComputeShader  = nullptr;

public:
	static void InitShader();
	static const SShader& FindShader(const char* path);
};