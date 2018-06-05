#include "SandEnginePCH.h"
#include "SRenderHelper.h"
#include "SandEngine/Resource/Material/Material.h"
#include "SandEngine/Application/SRenderer.h"
#include "SandEngine/Pipeline/SShader.h"
#include "SandBase/Log/Slog.h"
#include "SandEngine/Resource/Texture/STextureObject.h"

ID3D11Device*        SRenderHelper::g_Device           = nullptr;
IDXGISwapChain*      SRenderHelper::g_SwapChain        = nullptr;
ID3D11DeviceContext* SRenderHelper::g_ImmediateContext = nullptr;

void SRenderHelper::SetRenderState()
{

}

void SRenderHelper::AddMeshStream( SMeshBuffer * mesh , suInt32 mask )
{
	return;
}

void SRenderHelper::SetTexture( eShaderStage stage , int slot , STextureObject * tex )
{
	return;
}

bool SRenderHelper::CreateDeviceAndContext( HWND hwnd )
{
	IDXGIAdapter* pAdapter;
	SArray<IDXGIAdapter*> vAdapter;
	IDXGIFactory* pFactory = nullptr;

	if( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** )&pFactory ) ) )
	{
		return false;
	}

	for( suInt32 i = 0; pFactory->EnumAdapters( i , &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapter.PushBack( pAdapter );
	}

	if( pFactory )	pFactory->Release();

#ifdef _DEBUG
	// get adapter display modes:https://msdn.microsoft.com/en-us/library/windows/desktop/ff476878(v=vs.85).aspx
	IDXGIOutput* pOutput = nullptr;

	for( suInt32 i = 0 , numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		pAdapter = vAdapter[i];

		SLog::Info( "Adapter:%d" , i );

		SArray<IDXGIOutput*> outputList;
		suInt32			outputIdx = 0;
		IDXGIOutput*	pOutput = nullptr;
		while( pAdapter->EnumOutputs( outputIdx , &pOutput ) != DXGI_ERROR_NOT_FOUND )
		{
			outputList.PushBack( pOutput );
			outputIdx++;
		}

		for( suInt32 j = 0 , outputNum = outputList.GetSize(); j < outputNum; j++ )
		{
			SLog::Info( "Adapter Output:%d" , j );

			pOutput = outputList[j];

			suInt32 numMode = 0;
			DXGI_MODE_DESC* displayModes = NULL;
			DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , NULL ) ) )	continue;

			displayModes = new DXGI_MODE_DESC[numMode];

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , displayModes ) ) )	continue;

			for( suInt32 k = 0; k < numMode; k++ )
			{
				const DXGI_MODE_DESC& desc = displayModes[k];

				SLog::Info( "Format:%s width:%d height:%d refreshRate:%d/%d" , Enum2Str( desc.Format ) , desc.Width , desc.Height , desc.RefreshRate.Numerator , desc.RefreshRate.Denominator );
			}
		}
	}
#endif

	// create a device and immediate context: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476879(v=vs.85).aspx
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( &scd , sizeof( scd ) );
	scd.BufferCount                        = 1;
	scd.BufferDesc.Width                   = 0;
	scd.BufferDesc.Height                  = 0;
	scd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator   = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow                       = hwnd;
	scd.SampleDesc.Count                   = 1;
	scd.SampleDesc.Quality                 = 0;
	scd.Windowed                           = TRUE;
	scd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags                              = 0;

	D3D_FEATURE_LEVEL FeatureLevelsRequested[] = {	D3D_FEATURE_LEVEL_11_1 ,
													D3D_FEATURE_LEVEL_11_0 ,
													D3D_FEATURE_LEVEL_10_1 ,
													D3D_FEATURE_LEVEL_10_0 ,
													D3D_FEATURE_LEVEL_9_3 ,
													D3D_FEATURE_LEVEL_9_2 ,
													D3D_FEATURE_LEVEL_9_1 };

	suInt32 createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevelsSupported;

	bool success = false;
	for( suInt32 i = 0 , numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		HRESULT hr = D3D11CreateDeviceAndSwapChain( vAdapter[i] ,
													D3D_DRIVER_TYPE_UNKNOWN ,
													NULL ,
													createDeviceFlags ,
													FeatureLevelsRequested ,
													_countof( FeatureLevelsRequested ) ,
													D3D11_SDK_VERSION ,
													&scd ,
													&g_SwapChain ,
													&g_Device ,
													&FeatureLevelsSupported ,
													&g_ImmediateContext );

		if( hr == E_INVALIDARG )
		{
			hr = D3D11CreateDeviceAndSwapChain( vAdapter[i] ,
												D3D_DRIVER_TYPE_UNKNOWN ,
												NULL ,
												createDeviceFlags ,
												&FeatureLevelsRequested[1] ,
												_countof( FeatureLevelsRequested ) - 1 ,
												D3D11_SDK_VERSION ,
												&scd ,
												&g_SwapChain ,
												&g_Device ,
												&FeatureLevelsSupported ,
												&g_ImmediateContext );
		}

		if( hr == S_OK )
		{
			success = true;
			break;
		}
	}

	return success;
}

void SRenderHelper::CreateShader( SShader & shader )
{
	shader.CreateShader( g_Device );
}

void SRenderHelper::ReportDetailDebug()
{ 
#ifdef _DEBUG
	// ref: http://masterkenth.com/directx-leak-debugging/
	ID3D11Debug* pDebugDevice = nullptr;
	HRESULT hr = g_Device->QueryInterface( __uuidof( ID3D11Debug ) , reinterpret_cast< void** >( &pDebugDevice ) );
	if( hr == S_OK )
	{
		hr = pDebugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		SAFE_RELEASE( pDebugDevice );
	}
#endif
}