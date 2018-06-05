#include "SandEnginePCH.h"
#include "SandEngineModule.h"
#include "Callback/SCallback.h"
#include "GUI/SGui.h"
#include "SRenderer.h"
#include "SandEngine/Resource/SStaticMeshManager.h"
#include "SandEngine/Resource/Texture/STextureManager.h"

SCallbackManager	SandEngine::Callback;
SStaticMeshManager	SandEngine::StaticMeshManager;
STextureManager		SandEngine::TextureManager;

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
};

SandEngineHandler gSandEngineHandler;

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