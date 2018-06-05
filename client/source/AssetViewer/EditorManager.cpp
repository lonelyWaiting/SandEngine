#include "EditorManager.h"
#include "imguiDemo.h"
#include "SandEngine/Application/SandEngineModule.h"
#include "SandEngine/Resource/SStaticMeshManager.h"
#include "SandEngine/Callback/SCallback.h"
#include "SandEngine/GUI/SGui.h"

class GuiHandler : public SCallbackHandle
{
public:
	virtual void OnHandle( const SCallbackUserData& userData )
	{
		if( userData.pSender == &SandEngine::Callback.OnDropFile )
		{
			SandEngine::StaticMeshManager.LoadStaticMesh( ( const char* )userData.pUserData );
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineInit )
		{
			GuiRegister( imgui_demo , nullptr );
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineDeInit )
		{
			GuiUnRegister( imgui_demo , nullptr );
		}
	}
}g_GuiHandle;

void EditorInit()
{
	SandEngine::Callback.OnDropFile     += g_GuiHandle;
	SandEngine::Callback.OnEngineInit   += g_GuiHandle;
	SandEngine::Callback.OnEngineDeInit += g_GuiHandle;
}

void EditorDeInit()
{
	SandEngine::Callback.OnDropFile     -= g_GuiHandle;
	SandEngine::Callback.OnEngineInit   -= g_GuiHandle;
	SandEngine::Callback.OnEngineDeInit -= g_GuiHandle;
}