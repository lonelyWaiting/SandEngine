#include "SandEnginePCH.h"
#include "SApplication.h"
#include "SRenderer.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"
#include "SWindow.h"
#include "imgui/imgui.h"
#include "Callback/SCallback.h"
#include "SandEngineModule.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

LRESULT WindowProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	if( ImGui_ImplWin32_WndProcHandler( hwnd , msg , wParam , lParam ) )	return true;

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
			SCallbackUserData data;
			SVector2f size( ( float )LOWORD( lParam ) , ( float )HIWORD( lParam ) );
			data.pUserData = &size;
			SandEngine::Callback.OnResize.Trigger( &data );
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
					SCallbackUserData data;
					data.pUserData = lpszFile;
					SandEngine::Callback.OnDropFile.Trigger( &data );
				}
			}

			DragFinish( hDrop );

			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			struct ButtonDownInfo
			{
				HWND	  _hwnd;
				SVector2f _Pos;
			};

			ButtonDownInfo info;
			info._hwnd = hwnd;
			info._Pos = SVector2f((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));

			SCallbackUserData data;
			data.pUserData = &info;

			SandEngine::Callback.OnMouseDown.Trigger(&data);

			return 0;
		}

		case WM_RBUTTONUP:
		{
			SandEngine::Callback.OnMouseUp.Trigger();

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			struct mouseMoveInfo
			{
				SVector2f pos;
				WPARAM param;
			}info;

			info.pos   = SVector2f((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));;
			info.param = wParam;

			SCallbackUserData data;
			data.pUserData = &info;

			SandEngine::Callback.OnMouseMove.Trigger(&data);

			return 0;
		}
	}


	return DefWindowProc( hwnd , msg , wParam , lParam );
}

SApplication::SApplication()
{
	m_pWindow = nullptr;
}

bool SApplication::Init()
{
	SandEngineInit();

	m_pWindow = new SWindow( "SandEngine" , WindowProc , SVector2f( 1600 , 900 ) , SVector2f( 100 , 100 ) );
	if( !m_pWindow )	return false;

	if( !SRenderer::Get().Init( m_pWindow->GetHandle() ) )	return false;

	return true;
}

void SApplication::Shutdown()
{
	SandEngine::Callback.OnEngineDeInit.Trigger();
	SAFE_DELETE( m_pWindow );
	SRenderer::Get().Shutdown();
	SandEngineDeInit();
}

void SApplication::Render()
{
	SandEngine::Callback.OnUpdateScene.Trigger();

	SRenderer::Get().ClearColor( SVector4f( 75.f , 75.f , 75.f , 75.f ) );
	
	SandEngine::Callback.OnBeginRender.Trigger();
	SandEngine::Callback.OnEndRender.Trigger();
	
	SRenderer::Get().Present(0,0);
}

void SApplication::Run()
{
	SCallbackUserData userData;
	userData.pUserData = m_pWindow->GetHandle();
	SandEngine::Callback.OnEngineInit.Trigger( &userData );

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

		Render();
	}
}