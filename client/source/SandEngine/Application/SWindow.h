#pragma once

#include <SandBase\Math\SVector2f.h>
//class SVector2f;

class SWindow
{
public:
	SWindow( const char* caption , WNDPROC proc , const SVector2f& size , const SVector2f& pos );
	~SWindow();

public:
	SVector2f	GetCursorPosition();
	SVector2f	GetWindowSize();
	void		ResizeWindow( int width , int height );

	SVector2f	GetWindowPos();
	void		SetWindowPos( int x , int y );

	HWND		GetHandle();

private:
	void AdjustWindow();

private:
	HWND  m_hWnd;
	DWORD m_dStyle;
	SVector2f m_WindowSize;
	SVector2f m_WindowPos;
};
