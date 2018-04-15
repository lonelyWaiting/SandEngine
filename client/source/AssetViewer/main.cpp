#include "SandEngine\Application\SApplication.h"
#include <windows.h>
#include "EditorManager.h"

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPreInstance , LPSTR lpCmdLine , int nShowCmd )
{
	SApplication app;
	if( !app.Init() )	return -1;
	EditorInit();
	app.Run();
	EditorDeInit();

	return 1;
}