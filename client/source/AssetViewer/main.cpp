#include "SandEngine\Application\SApplication.h"
#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPreInstance , LPSTR lpCmdLine , int nShowCmd )
{
	SApplication app;
	if( !app.Init() )	return -1;
	app.Run();

	return 1;
}