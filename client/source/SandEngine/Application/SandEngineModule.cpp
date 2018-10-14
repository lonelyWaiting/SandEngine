#include "SandEnginePCH.h"
#include "SandEngineModule.h"
#include "Callback/SCallback.h"
#include "GUI/SGui.h"
#include "SRenderer.h"
#include "SandEngine/Resource/Shaders/SShader.h"

SCallbackManager	SandEngine::Callback;

class SandEngineHandler : public SCallbackHandle
{
public:
	void OnHandle( const SCallbackUserData& userData )
	{
		if( userData.pSender == &SandEngine::Callback.OnBeginRender )
		{
			ImGuiBeginRender();
			GuiUpdate();
		}
		else if( userData.pSender == &SandEngine::Callback.OnEndRender )
		{
			ImGuiEndRender();
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineInit )
		{
			ImGuiInit( userData.pUserData );
			SShader::InitShader();
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineDeInit )
		{
			ImGuiShutdown();
		}
		else if( userData.pSender == &SandEngine::Callback.OnUpdateScene )
		{
			
		}
		else if( userData.pSender == &SandEngine::Callback.OnResize )
		{
			ImGuiBeginResize();
			SRenderer::Get().Resize(*(const SVector2f*)(userData.pUserData));
			ImGuiEndResize();
		}
	}
}gSandEngineHandler;


void SandEngineInit()
{
	SandEngine::Callback.OnBeginRender           += gSandEngineHandler;
	SandEngine::Callback.OnEndRender             += gSandEngineHandler;
	SandEngine::Callback.OnEngineInit            += gSandEngineHandler;
	SandEngine::Callback.OnEngineDeInit          += gSandEngineHandler;
	SandEngine::Callback.OnUpdateScene           += gSandEngineHandler;
	SandEngine::Callback.OnResize                += gSandEngineHandler;
}

void SandEngineDeInit()
{
	SandEngine::Callback.OnBeginRender           -= gSandEngineHandler;
	SandEngine::Callback.OnEndRender             -= gSandEngineHandler;
	SandEngine::Callback.OnEngineInit            -= gSandEngineHandler;
	SandEngine::Callback.OnEngineDeInit          -= gSandEngineHandler;
	SandEngine::Callback.OnUpdateScene           -= gSandEngineHandler;
	SandEngine::Callback.OnResize                -= gSandEngineHandler;
}