#include "SandEnginePCH.h"
#include "SApplication.h"
#include "SRenderer.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector4f.h"
#include "SWindow.h"
#include "GUI/SGui.h"
#include "imgui/imgui.h"

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
			ImGuiBeginResize();
			SRenderer::Get().Resize( SVector2f( ( float )LOWORD( lParam ) , ( float )HIWORD( lParam ) ) );
			ImGuiEndResize();
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

bool SApplication::Init()
{
	m_pWindow = new SWindow( "SandEngine" , WindowProc , SVector2f( 1600 , 900 ) , SVector2f( 100 , 100 ) );
	if( !m_pWindow )	return false;

	if( !SRenderer::Get().Init( m_pWindow->GetHandle() ) )	return false;

	ImGuiInit( m_pWindow->GetHandle() , SRenderer::Get().GetDevice() , SRenderer::Get().GetDeviceContext() );

	return true;
}

void SApplication::Shutdown()
{
	SAFE_DELETE( m_pWindow );
	SRenderer::Get().Shutdown();
	ImGuiShutdown();
}

void GuiTest()
{
	static bool show_demo_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImVec4( 0.45f , 0.55f , 0.60f , 1.00f );
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text( "Hello, world!" );                           // Display some text (you can use a format string too)
		ImGui::SliderFloat( "float" , &f , 0.0f , 1.0f );            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3( "clear color" , ( float* )&clear_color ); // Edit 3 floats representing a color

		ImGui::Checkbox( "Demo Window" , &show_demo_window );      // Edit bools storing our windows open/close state
		ImGui::Checkbox( "Another Window" , &show_another_window );

		if( ImGui::Button( "Button" ) )                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text( "counter = %d" , counter );

		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)" , 1000.0f / ImGui::GetIO().Framerate , ImGui::GetIO().Framerate );
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if( show_another_window )
	{
		ImGui::Begin( "Another Window" , &show_another_window );
		ImGui::Text( "Hello from another window!" );
		if( ImGui::Button( "Close Me" ) )
			show_another_window = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if( show_demo_window )
	{
		ImGui::SetNextWindowPos( ImVec2( 650 , 20 ) , ImGuiCond_FirstUseEver ); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow( &show_demo_window );
	}
}

void SApplication::Render()
{
	SRenderer::Get().ClearColor( SVector4f( 75.f , 75.f , 75.f , 75.f ) );
	ImGuiBeginRender();

	GuiTest();

	ImGuiEndRender();
	SRenderer::Get().Present(0,0);
}

void SApplication::Update()
{
	
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
		Render();
	}
}