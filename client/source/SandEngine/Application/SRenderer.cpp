#include "SandEnginePCH.h"
#include "SRenderer.h"
#include "SandBase/Vector/SArray.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"

const char* outputDXGIFormat( const DXGI_FORMAT& format )
{
	switch( format )
	{
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			return "R32G32B32A32_Typeless";
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return "R32G32B32A32_Float";
		case DXGI_FORMAT_R32G32B32A32_UINT :
			return "R32G32B32A32_Uint";
		case DXGI_FORMAT_R32G32B32A32_SINT :
			return "R32G32B32A32_SInt";
		case DXGI_FORMAT_R32G32B32_TYPELESS :
			return "R32G32B32_Typeless";
		case DXGI_FORMAT_R32G32B32_FLOAT :
			return "R32G32B32_Float";
		case DXGI_FORMAT_R32G32B32_UINT :
			return "R32G32B32_Uint";
		case DXGI_FORMAT_R32G32B32_SINT :
			return "R32G32B32_Sint";
		case DXGI_FORMAT_R16G16B16A16_TYPELESS :
			return "R16G16B16A16_Typeless";
		case DXGI_FORMAT_R16G16B16A16_FLOAT :
			return "R16G16B16A16_Float";
		case DXGI_FORMAT_R16G16B16A16_UNORM :
			return "R16G16B16A16_UNORM";
		case DXGI_FORMAT_R16G16B16A16_UINT :
			return "R16G16B16A16_UINT";
		case DXGI_FORMAT_R16G16B16A16_SNORM :
			return "R16G16B16A16_SNORM";
		case DXGI_FORMAT_R16G16B16A16_SINT :
			return "R16G16B16A16_SINT";
		case DXGI_FORMAT_R32G32_TYPELESS :
			return "R32G32_Typeless";
		case DXGI_FORMAT_R32G32_FLOAT :
			return "R32G32_Float";
		case DXGI_FORMAT_R32G32_UINT :
			return "R32G32_UINT";
		case DXGI_FORMAT_R32G32_SINT :
			return "R32G32_SINT";
		case DXGI_FORMAT_R32G8X24_TYPELESS :
			return "R32G8X24_TYPELESS";
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT :
			return "D32_Float_S8X24_UINT";
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS :
			return "R32_Float_X8X24_Typeless";
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT :
			return "X32_TYPELESS_G8X24_UINT";
		case DXGI_FORMAT_R10G10B10A2_TYPELESS :
			return "R10G10B10A2_TYPELESS";
		case DXGI_FORMAT_R10G10B10A2_UNORM :
			return "R10G10B10A2_UNORM";
		case DXGI_FORMAT_R10G10B10A2_UINT :
			return "R10G10B10A2_UINT";
		case DXGI_FORMAT_R11G11B10_FLOAT :
			return "R11G11B10_FLOAT";
		case DXGI_FORMAT_R8G8B8A8_TYPELESS :
			return "R8G8B8A8_TYPELESS";
		case DXGI_FORMAT_R8G8B8A8_UNORM :
			return "R8G8B8A8_UNORM";
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB :
			return "R8G8B8A8_UNORM_SRGB";
		case DXGI_FORMAT_R8G8B8A8_UINT :
			return "R8G8B8A8_UINT";
		case DXGI_FORMAT_R8G8B8A8_SNORM :
			return "R8G8B8A8_SNORM";
		case DXGI_FORMAT_R8G8B8A8_SINT :
			return "R8G8B8A8_SINT";
		case DXGI_FORMAT_R16G16_TYPELESS :
			return "R16G16_TYPELESS";
		case DXGI_FORMAT_R16G16_FLOAT :
			return "R16G16_FLOAT";
		case DXGI_FORMAT_R16G16_UNORM :
			return "R16G16_UNORM";
		case DXGI_FORMAT_R16G16_UINT :
			return "R16G16_UINT";
		case DXGI_FORMAT_R16G16_SNORM :
			return "R16G16_SNORM";
		case DXGI_FORMAT_R16G16_SINT :
			return "R16G16_SINT";
		case DXGI_FORMAT_R32_TYPELESS :
			return "R32_TYPELESS";
		case DXGI_FORMAT_D32_FLOAT :
			return "D32_FLOAT";
		case DXGI_FORMAT_R32_FLOAT :
			return "R32_FLOAT";
		case DXGI_FORMAT_R32_UINT :
			return "R32_UINT";
		case DXGI_FORMAT_R32_SINT :
			return "R32_SINT";
		case DXGI_FORMAT_R24G8_TYPELESS :
			return "R24G8_TYPELESS";
		case DXGI_FORMAT_D24_UNORM_S8_UINT :
			return "D24_UNORM_S8_UINT";
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS :
			return "R24_UNORM_X8_TYPELESS";
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT :
			return "X24_TYPELESS_G8_UINT";
		case DXGI_FORMAT_R8G8_TYPELESS :
			return "R8G8_TYPELESS";
		case DXGI_FORMAT_R8G8_UNORM :
			return "R8G8_UNORM";
		case DXGI_FORMAT_R8G8_UINT :
			return "R8G8_UINT";
		case DXGI_FORMAT_R8G8_SNORM :
			return "R8G8_SNORM";
		case DXGI_FORMAT_R8G8_SINT :
			return "R8G8_SINT";
		case DXGI_FORMAT_R16_TYPELESS :
			return "R16_TYPELESS";
		case DXGI_FORMAT_R16_FLOAT :
			return "R16_FLOAT";
		case DXGI_FORMAT_D16_UNORM :
			return "D16_UNORM";
		case DXGI_FORMAT_R16_UNORM :
			return "R16_UNORM";
		case DXGI_FORMAT_R16_UINT :
			return "R16_UINT";
		case DXGI_FORMAT_R16_SNORM :
			return "R16_SNORM";
		case DXGI_FORMAT_R16_SINT :
			return "R16_SINT";
		case DXGI_FORMAT_R8_TYPELESS :
			return "R8_TYPELESS";
		case DXGI_FORMAT_R8_UNORM :
			return "R8_UNORM";
		case DXGI_FORMAT_R8_UINT :
			return "R8_UINT";
		case DXGI_FORMAT_R8_SNORM :
			return "R8_SNORM";
		case DXGI_FORMAT_R8_SINT :
			return "R8_SINT";
		case DXGI_FORMAT_A8_UNORM :
			return "A8_UNORM";
		case DXGI_FORMAT_R1_UNORM :
			return "R1_UNORM";
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP :
			return "R9G9B9E5_SHAREDEXP";
		case DXGI_FORMAT_R8G8_B8G8_UNORM :
			return "R8G8_B8G8_UNORM";
		case DXGI_FORMAT_G8R8_G8B8_UNORM :
			return "G8R8_G8B8_UNORM";
		case DXGI_FORMAT_BC1_TYPELESS :
			return "BC1_TYPELESS";
		case DXGI_FORMAT_BC1_UNORM :
			return "BC1_UNORM";
		case DXGI_FORMAT_BC1_UNORM_SRGB :
			return "BC1_UNORM_SRGB";
		case DXGI_FORMAT_BC2_TYPELESS :
			return "BC2_TYPELESS";
		case DXGI_FORMAT_BC2_UNORM :
			return "BC2_UNORM";
		case DXGI_FORMAT_BC2_UNORM_SRGB :
			return "BC2_UNORM_SRGB";
		case DXGI_FORMAT_BC3_TYPELESS :
			return "BC3_TYPELESS";
		case DXGI_FORMAT_BC3_UNORM :
			return "BC3_UNORM";
		case DXGI_FORMAT_BC3_UNORM_SRGB :
			return "BC3_UNORM_SRGB";
		case DXGI_FORMAT_BC4_TYPELESS :
			return "BC4_TYPELESS";
		case DXGI_FORMAT_BC4_UNORM :
			return "BC4_UNORM";
		case DXGI_FORMAT_BC4_SNORM :
			return "BC4_SNORM";
		case DXGI_FORMAT_BC5_TYPELESS :
			return "BC5_TYPELESS";
		case DXGI_FORMAT_BC5_UNORM :
			return "BC5_UNORM";
		case DXGI_FORMAT_BC5_SNORM :
			return "BC5_SNORM";
		case DXGI_FORMAT_B5G6R5_UNORM :
			return "B5G6R5_UNORM";
		case DXGI_FORMAT_B5G5R5A1_UNORM :
			return "B5G5R5A1_UNORM";
		case DXGI_FORMAT_B8G8R8A8_UNORM :
			return "B8G8R8A8_UNORM";
		case DXGI_FORMAT_B8G8R8X8_UNORM :
			return "B8G8R8X8_UNORM";
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM :
			return "R10G10B10_XR_BIAS_A2_UNORM";
		case DXGI_FORMAT_B8G8R8A8_TYPELESS :
			return "B8G8R8A8_TYPELESS";
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB :
			return "B8G8R8A8_UNORM_SRGB";
		case DXGI_FORMAT_B8G8R8X8_TYPELESS :
			return "B8G8R8X8_TYPELESS";
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB :
			return "B8G8R8X8_UNORM_SRGB";
		case DXGI_FORMAT_BC6H_TYPELESS :
			return "BC6H_TYPELESS";
		case DXGI_FORMAT_BC6H_UF16 :
			return "BC6H_UF16";
		case DXGI_FORMAT_BC6H_SF16 :
			return "BC6H_SF16";
		case DXGI_FORMAT_BC7_TYPELESS :
			return "BC7_TYPELESS";
		case DXGI_FORMAT_BC7_UNORM :
			return "BC7_UNORM";
		case DXGI_FORMAT_BC7_UNORM_SRGB :
			return "BC7_UNORM_SRGB";
		case DXGI_FORMAT_AYUV :
			return "AYUV";
		case DXGI_FORMAT_Y410 :
			return "Y410";
		case DXGI_FORMAT_Y416 :
			return "Y416";
		case DXGI_FORMAT_NV12 :
			return "NV12";
		case DXGI_FORMAT_P010 :
			return "P010";
		case DXGI_FORMAT_P016 :
			return "P016";
		case DXGI_FORMAT_420_OPAQUE :
			return "420_OPAQUE";
		case DXGI_FORMAT_YUY2 :
			return "YUY2";
		case DXGI_FORMAT_Y210 :
			return "Y210";
		case DXGI_FORMAT_Y216 :
			return "Y216";
		case DXGI_FORMAT_NV11 :
			return "NV11";
		case DXGI_FORMAT_AI44 :
			return "AI44";
		case DXGI_FORMAT_IA44 :
			return "IA44";
		case DXGI_FORMAT_P8 :
			return "P8";
		case DXGI_FORMAT_A8P8 :
			return "A8P8";
		case DXGI_FORMAT_B4G4R4A4_UNORM :
			return "B4G4R4A4_UNORM";
		case DXGI_FORMAT_P208 :
			return "P208";
		case DXGI_FORMAT_V208 :
			return "V208";
		case DXGI_FORMAT_V408 :
			return "V408";
		case DXGI_FORMAT_FORCE_UINT :
			return "FORCE_UINT";
		default:
			return "UNKNOWN";
	}
}

SRenderer & SRenderer::Get()
{
	static SRenderer renderer;
	return renderer;
}

bool SRenderer::Init( HWND hwnd , const SVector2f & winSize )
{
	// enumerate adapters:https://msdn.microsoft.com/en-us/library/windows/desktop/ff476877(v=vs.85).aspx
	IDXGIAdapter* pAdapter;
	SArray<IDXGIAdapter*> vAdapter;
	IDXGIFactory* pFactory = nullptr;

	if( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** )&pFactory ) ) )
	{
		return false;
	}

	for( uint32 i = 0; pFactory->EnumAdapters( i , &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapter.PushBack( pAdapter );
	}

	if( pFactory )	pFactory->Release();

#ifdef _DEBUG
	// get adapter display modes:https://msdn.microsoft.com/en-us/library/windows/desktop/ff476878(v=vs.85).aspx
	IDXGIOutput* pOutput = nullptr;
	
	for( uint32 i = 0 , numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		pAdapter = vAdapter[i];

		std::cout << "Adapter:" << i << std::endl;

		SArray<IDXGIOutput*> outputList;
		uint32			outputIdx = 0;
		IDXGIOutput*	pOutput   = nullptr;
		while( pAdapter->EnumOutputs( outputIdx , &pOutput ) != DXGI_ERROR_NOT_FOUND )
		{
			outputList.PushBack( pOutput );
			outputIdx++;
		}

		for( uint32 j = 0 , outputNum = outputList.GetSize(); j < outputNum; j++ )
		{
			std::cout << "Adapter Output:" << j << std::endl;

			pOutput = outputList[j];
			
			uint32 numMode               = 0;
			DXGI_MODE_DESC* displayModes = NULL;
			DXGI_FORMAT format           = DXGI_FORMAT_R32G32B32A32_FLOAT;

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , NULL ) ) )	continue;

			displayModes = new DXGI_MODE_DESC[numMode];

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , displayModes ) ) )	continue;

			for( uint32 k = 0; k < numMode; k++ )
			{
				const DXGI_MODE_DESC& desc = displayModes[k];

				std::cout << "Format:" << outputDXGIFormat( desc.Format ) << "width:" << desc.Width << "height:" << desc.Height << "refreshRate:" << desc.RefreshRate.Numerator << "," << desc.RefreshRate.Denominator << std::endl;
			}
		}
	}
#endif

	// create a device and immediate context: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476879(v=vs.85).aspx
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( &scd , sizeof( scd ) );
	scd.BufferCount                        = 2;
	scd.BufferDesc.Width                   = ( const uint32 )winSize.x;
	scd.BufferDesc.Height                  = ( const uint32 )winSize.y;
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

	uint32 createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevelsSupported;

	bool success = false;
	for( uint32 i = 0, numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		HRESULT hr = D3D11CreateDeviceAndSwapChain( vAdapter[i] ,
													D3D_DRIVER_TYPE_UNKNOWN ,
													NULL ,
													createDeviceFlags ,
													FeatureLevelsRequested ,
													_countof( FeatureLevelsRequested ) ,
													D3D11_SDK_VERSION ,
													&scd ,
													&m_pSwapChain ,
													&m_pDevice ,
													&FeatureLevelsSupported ,
													&m_pImmediateContext );

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
												&m_pSwapChain ,
												&m_pDevice ,
												&FeatureLevelsSupported ,
												&m_pImmediateContext );
		}

		if( hr == S_OK )
		{
			success = true;
			break;
		}
	}

	if( !success )	return false;

	ID3D11Texture2D* pBackBuffer = nullptr;
	if( FAILED( m_pSwapChain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( LPVOID* )&pBackBuffer ) ) )	return false;
	if( FAILED( m_pDevice->CreateRenderTargetView( pBackBuffer , NULL , &m_pRenderTargetView ) ) )	return false;
	ULONG refCount = pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width              = ( const uint32 )winSize.x;
	dsd.Height             = ( const uint32 )winSize.y;
	dsd.MipLevels          = 1;
	dsd.ArraySize          = 1;
	dsd.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count   = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage              = D3D11_USAGE_DEFAULT;
	dsd.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags     = 0;
	dsd.MiscFlags          = 0;

	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	if( FAILED( m_pDevice->CreateTexture2D( &dsd , 0 , &pDepthStencilBuffer ) ) )	return false;
	if( FAILED( m_pDevice->CreateDepthStencilView( pDepthStencilBuffer , 0 , &m_pDepthStencilView ) ) )	return false;
	pDepthStencilBuffer->Release();

	// setup a direct3d context to render into a swap chain
	m_pImmediateContext->OMSetRenderTargets( 1 , &m_pRenderTargetView , m_pDepthStencilView );

	D3D11_VIEWPORT vp;
	vp.Width    = winSize.x;
	vp.Height   = winSize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports( 1 , &vp );

	return true;
}

void SRenderer::Resize( const SVector2f& size )
{
	if( !m_pImmediateContext || !m_pSwapChain )	return;

	// d3d11 handle windows resize:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075%28v=vs.85%29.aspx#Handling_Window_Resizing

	m_pImmediateContext->OMSetRenderTargets( 0 , 0 , 0 );

	// Release all outstanding references to the swap chain's buffers
	m_pRenderTargetView->Release();

	// Preserve the existing buffer count and format
	// Automatically choose the width and height to match the client rect for HWND
	HRESULT hr = m_pSwapChain->ResizeBuffers( 0 , 0 , 0 , DXGI_FORMAT_UNKNOWN , 0 );
	
	// Perform error handling here!
	if( FAILED( hr ) )
	{
		std::cout << "Resize Buffer Failed" << std::endl;
		return;
	}

	// Get buffer and create a render-target-view
	ID3D11Texture2D* pBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( void** )&pBuffer );

	// Perform error handling here!
	if( FAILED( hr ) )
	{
		std::cout << "Resize Get Buffer Failed" << std::endl;
		return;
	}

	hr = m_pDevice->CreateRenderTargetView( pBuffer , nullptr , &m_pRenderTargetView );
	if( FAILED( hr ) )
	{
		std::cout << "Resize CreateRenderTargetView Failed" << std::endl;
		return;
	}

	pBuffer->Release();

	// should i recreate the depth/stencil buffer too ?
	// https://gamedev.stackexchange.com/questions/86164/idxgiswapchainresizebuffers-should-i-recreate-the-depth-stencil-buffer-too
	// depth/stencil resize is just optional
	m_pDepthStencilView->Release();

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width              = ( const uint32 )size.x;
	dsd.Height             = ( const uint32 )size.y;
	dsd.MipLevels          = 1;
	dsd.ArraySize          = 1;
	dsd.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count   = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage              = D3D11_USAGE_DEFAULT;
	dsd.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags     = 0;
	dsd.MiscFlags          = 0;

	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	if( FAILED( m_pDevice->CreateTexture2D( &dsd , 0 , &pDepthStencilBuffer ) ) )	return;
	if( FAILED( m_pDevice->CreateDepthStencilView( pDepthStencilBuffer , 0 , &m_pDepthStencilView ) ) )	return;
	pDepthStencilBuffer->Release();

	m_pImmediateContext->OMSetRenderTargets( 1 , &m_pRenderTargetView , m_pDepthStencilView );

	D3D11_VIEWPORT vp;
	vp.Width    = size.x;
	vp.Height   = size.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_pImmediateContext->RSSetViewports( 1 , &vp );
}

void SRenderer::ClearColor( const SVector4f & color )
{
	const float c[4] = { color[0] / 255.0f ,color[1] / 255.0f,color[2] / 255.0f,color[3] / 255.0f };

	m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView , c );
	m_pImmediateContext->ClearDepthStencilView( m_pDepthStencilView , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );
}

void SRenderer::Shutdown()
{
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pDepthStencilView );
	SAFE_RELEASE( m_pImmediateContext );

#ifdef _DEBUG
	// ref: http://masterkenth.com/directx-leak-debugging/
	ID3D11Debug* pDebugDevice = nullptr;
	HRESULT hr = m_pDevice->QueryInterface( __uuidof( ID3D11Debug ) , ( void** )&pDebugDevice );
	if( hr == S_OK )
	{
		hr = pDebugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		SAFE_RELEASE( pDebugDevice );
	}
#endif
	SAFE_RELEASE( m_pDevice );
}

void SRenderer::Present( uint32 syncInterval /*= 0*/, uint32 presentFlag /*= 0*/ )
{
	if( m_pSwapChain )	m_pSwapChain->Present( syncInterval , presentFlag );
}

ID3D11Device* SRenderer::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* SRenderer::GetDeviceContext()
{
	return m_pImmediateContext;
}