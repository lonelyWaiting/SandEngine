#include "SandEnginePCH.h"
#include "SWindow.h"
#include <windows.h>

SWindow::SWindow( const char * caption , WNDPROC proc , const SVector2f& size , const SVector2f& pos )
{
	m_hWnd = 0;
	m_WindowSize = size;
	m_WindowPos  = pos;
	m_dStyle     = ( WS_OVERLAPPEDWINDOW | WS_VISIBLE );

	WNDCLASSEX wc;
	memset( &wc , 0 , sizeof( wc ) );
	wc.cbSize        = sizeof( WNDCLASSEX );
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = proc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = 0;
	wc.hIcon         = LoadIcon( NULL , IDI_APPLICATION );
	wc.hCursor       = LoadCursor( NULL , IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = caption;
	wc.hIconSm       = LoadIcon( NULL , IDI_APPLICATION );

	RegisterClassEx( &wc );

	m_hWnd = CreateWindowEx( NULL ,
							 wc.lpszClassName ,
							 caption ,
							 m_dStyle ,
							 ( int32 )m_WindowPos.x ,
							 ( int32 )m_WindowPos.y ,
							 ( int32 )m_WindowSize.x ,
							 ( int32 )m_WindowSize.y ,
							 NULL ,
							 NULL ,
							 NULL ,
							 NULL );

	ShowWindow( m_hWnd , SW_NORMAL );
	UpdateWindow( m_hWnd );

	/*
		http://lonelywaiting.github.io/Win32-Application%E5%BC%80%E5%90%AFConsole%E7%AA%97%E5%8F%A3/
	*/
	AllocConsole();
	*stdin  = *_fdopen( _open_osfhandle( ( intptr_t )GetStdHandle( STD_INPUT_HANDLE )  , _O_TEXT ) , "r" );
	*stdout = *_fdopen( _open_osfhandle( ( intptr_t )GetStdHandle( STD_OUTPUT_HANDLE ) , _O_TEXT ) , "w" );
	*stderr = *_fdopen( _open_osfhandle( ( intptr_t )GetStdHandle( STD_ERROR_HANDLE )  , _O_TEXT ) , "w" );
}

SWindow::~SWindow()
{
	if( m_hWnd )	DestroyWindow( m_hWnd );
	m_hWnd = NULL;
}

SVector2f SWindow::GetCursorPosition()
{
	POINT p;
	GetCursorPos( &p );
	ScreenToClient( m_hWnd , &p );

	return SVector2f( static_cast< float >( p.x ) , static_cast< float >( p.y ) );
}

SVector2f SWindow::GetWindowSize()
{
	return m_WindowSize;
}

void SWindow::ResizeWindow( int width , int height )
{}

SVector2f SWindow::GetWindowPos()
{
	return m_WindowPos;
}

void SWindow::SetWindowPos( int x , int y )
{
	m_WindowPos.x = ( float )x;
	m_WindowPos.y = ( float )y;

	AdjustWindow();
}

HWND SWindow::GetHandle()
{
	return m_hWnd;
}

void SWindow::AdjustWindow()
{
	RECT rect;
	rect.left   = ( LONG )m_WindowPos.x;
	rect.top    = ( LONG )m_WindowPos.y;
	rect.right  = ( LONG )( m_WindowPos.x + m_WindowSize.x );
	rect.bottom = ( LONG )( m_WindowPos.y + m_WindowSize.y );
	AdjustWindowRect( &rect , m_dStyle , false );
}
