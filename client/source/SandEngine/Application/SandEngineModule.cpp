#include "SandEnginePCH.h"
#include "SandEngineModule.h"
#include "Callback/SCallback.h"
#include "GUI/SGui.h"
#include "SRenderer.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandBase/Math/SVector2f.h"
#include "SandEngine/Sky/skybox.h"
#include "sEventInfo.h"

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
			SRenderHelper::BeginNewFrame();
			SkyboxRender();
		}
		else if( userData.pSender == &SandEngine::Callback.OnEndRender )
		{
			ImGuiEndRender();
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineInit )
		{
			ImGuiInit( userData.pUserData );
			SRenderHelper::Init();
			SkyboxInit("../data/textures/anniversary_lounge_1k.hdr");
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
		else if (userData.pSender == &SandEngine::Callback.OnMouseDown)
		{
			MouseEventInfo& info = *(MouseEventInfo*)userData.pUserData;
			SetCapture(info.hwnd);
		}
		else if (userData.pSender == &SandEngine::Callback.OnMouseUp)
		{
			ReleaseCapture();
		}
		else if (userData.pSender == &SandEngine::Callback.OnMouseMove)
		{
			MouseEventInfo& info = *(MouseEventInfo*)userData.pUserData;
			if (info.mouseType == eMB_Right)
			{
				SRenderer::Get().GetMainCamera().Rotate(info.offset.x, info.offset.y);
			}
		}
		else if (userData.pSender == &SandEngine::Callback.OnKeyPressed)
		{
			sKeyEventInfo & info = *(sKeyEventInfo*)userData.pUserData;
			if (info.keyCode == eKeyCode::W)
			{
				SRenderer::Get().GetMainCamera().MoveFront(1.0f);
			}
			else if (info.keyCode == eKeyCode::S)
			{
				SRenderer::Get().GetMainCamera().MoveFront(-1.0f);
			}
			else if (info.keyCode == eKeyCode::A)
			{
				SRenderer::Get().GetMainCamera().MoveRight(-1.0f);
			}
			else if (info.keyCode == eKeyCode::D)
			{
				SRenderer::Get().GetMainCamera().MoveRight(1.0f);
			}

		}
		else if (userData.pSender == &SandEngine::Callback.OnKeyRelease)
		{

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
	SandEngine::Callback.OnMouseDown             += gSandEngineHandler;
	SandEngine::Callback.OnMouseUp               += gSandEngineHandler;
	SandEngine::Callback.OnMouseMove             += gSandEngineHandler;
	SandEngine::Callback.OnKeyPressed            += gSandEngineHandler;
	SandEngine::Callback.OnKeyRelease            += gSandEngineHandler;
}

void SandEngineDeInit()
{
	SandEngine::Callback.OnBeginRender           -= gSandEngineHandler;
	SandEngine::Callback.OnEndRender             -= gSandEngineHandler;
	SandEngine::Callback.OnEngineInit            -= gSandEngineHandler;
	SandEngine::Callback.OnEngineDeInit          -= gSandEngineHandler;
	SandEngine::Callback.OnUpdateScene           -= gSandEngineHandler;
	SandEngine::Callback.OnResize                -= gSandEngineHandler;
	SandEngine::Callback.OnMouseDown             -= gSandEngineHandler;
	SandEngine::Callback.OnMouseUp               -= gSandEngineHandler;
	SandEngine::Callback.OnMouseMove             -= gSandEngineHandler;
	SandEngine::Callback.OnKeyPressed            -= gSandEngineHandler;
	SandEngine::Callback.OnKeyRelease            -= gSandEngineHandler;
}