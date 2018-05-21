#include "SandEnginePCH.h"
#include "SRenderer.h"
#include "SandBase/Vector/SArray.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"
#include "SandBase/Log/SLog.h"

SRenderer & SRenderer::Get()
{
	static SRenderer renderer;
	return renderer;
}

bool SRenderer::Init( HWND hwnd )
{
	// enumerate adapters:https://msdn.microsoft.com/en-us/library/windows/desktop/ff476877(v=vs.85).aspx
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
		IDXGIOutput*	pOutput   = nullptr;
		while( pAdapter->EnumOutputs( outputIdx , &pOutput ) != DXGI_ERROR_NOT_FOUND )
		{
			outputList.PushBack( pOutput );
			outputIdx++;
		}

		for( suInt32 j = 0 , outputNum = outputList.GetSize(); j < outputNum; j++ )
		{
			SLog::Info( "Adapter Output:%d" , j );

			pOutput = outputList[j];
			
			suInt32 numMode               = 0;
			DXGI_MODE_DESC* displayModes = NULL;
			DXGI_FORMAT format           = DXGI_FORMAT_R32G32B32A32_FLOAT;

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

	RECT clientArea;
	GetClientRect( hwnd , &clientArea );

	int clientWidth  = clientArea.right - clientArea.left;
	int clientHeight = clientArea.bottom - clientArea.top;

	// create a device and immediate context: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476879(v=vs.85).aspx
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( &scd , sizeof( scd ) );
	scd.BufferCount                        = 2;
	scd.BufferDesc.Width                   = clientWidth;
	scd.BufferDesc.Height                  = clientHeight;
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
	for( suInt32 i = 0, numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
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
	dsd.Width              = clientWidth;
	dsd.Height             = clientHeight;
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
	vp.Width    = ( float )clientWidth;
	vp.Height   = ( float )clientHeight;
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
	if( m_pRenderTargetView )	m_pRenderTargetView->Release();

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
	if( m_pDepthStencilView )	m_pDepthStencilView->Release();

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width              = ( const suInt32 )size.x;
	dsd.Height             = ( const suInt32 )size.y;
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
	HRESULT hr = m_pDevice->QueryInterface( __uuidof( ID3D11Debug ) , reinterpret_cast< void** >( &pDebugDevice ) );
	if( hr == S_OK )
	{
		hr = pDebugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		SAFE_RELEASE( pDebugDevice );
	}
#endif

	SAFE_RELEASE( m_pDevice );
}

void SRenderer::Present( suInt32 syncInterval /*= 0*/, suInt32 presentFlag /*= 0*/ )
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