#include "SandEngine\Application\SApplication.h"
#include <windows.h>
#include "EditorManager.h"
#include "IBL.h"

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPreInstance , LPSTR lpCmdLine , int nShowCmd )
{
	SApplication app;
	if( !app.Init() )	return -1;
	EditorInit();
	IBLInit();
	app.Run();
	EditorDeInit();
	IBLDeInit();

	return 1;
}