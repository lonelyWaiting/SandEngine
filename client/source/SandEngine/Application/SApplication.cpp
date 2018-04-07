#include "SandEnginePCH.h"
#include "SApplication.h"
#include "SRenderer.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"
#include "SWindow.h"

LRESULT WindowProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	//if( ImWndProHandler( hwnd , msg , wParam , lParam ) )	return true;

	switch( msg )
	{
		case WM_CLOSE:
		{
			DestroyWindow( hwnd );
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_SIZE:
		{
			SRenderer::Get().Resize( SVector2f( ( float )LOWORD( lParam ) , ( float )HIWORD( lParam ) ) );
			return 0;
		}

		case WM_DROPFILES:
		{
			HDROP hDrop = ( HDROP )( wParam );
			TCHAR lpszFile[MAX_PATH];

			UINT nCount = DragQueryFile( hDrop , 0xFFFFFFFF , NULL , 0 );

			for( UINT i = 0; i < nCount; i++ )
			{
				if( DragQueryFile( hDrop , i , lpszFile , MAX_PATH ) > 0 )
				{
					//AssetViewer::LoadAsset( lpszFile );
				}
			}

			DragFinish( hDrop );

			return 0;
		}
	}


	return DefWindowProc( hwnd , msg , wParam , lParam );
}

SApplication::SApplication()
{
	m_pWindow = nullptr;
}

SApplication::~SApplication()
{
	Shutdown();
}

bool SApplication::Init()
{
	SVector2f winSize( 1600 , 900 );

	m_pWindow = new SWindow( "SandEngine" , WindowProc , winSize , SVector2f( 100 , 100 ) );
	if( !m_pWindow )	return false;

	return SRenderer::Get().Init( m_pWindow->GetHandle() , winSize );
}

void SApplication::Shutdown()
{
	SAFE_DELETE( m_pWindow );
}

void SApplication::Update()
{
	SRenderer::Get().ClearColor( SVector4f( 75.f , 75.f , 75.f , 0.0f ) );

	SRenderer::Get().Present();
}

void SApplication::Run()
{
	MSG msg;
	while( true )
	{
		while( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
			{
				Shutdown();
				return;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		Update();
	}
}