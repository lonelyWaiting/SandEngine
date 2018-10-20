#include "SandEnginePCH.h"
#include "SandEngineModule.h"
#include "Callback/SCallback.h"
#include "GUI/SGui.h"
#include "SRenderer.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandBase/Math/SVector2f.h"
#include "SandEngine/Sky/skybox.h"

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
			struct ButtonDownInfo
			{
				HWND	  _hwnd;
				SVector2f _pos;
			};

			ButtonDownInfo& info = *(ButtonDownInfo*)userData.pUserData;

			SRenderer::Get().GetMainCamera().SetLastMousePressPos(info._pos);

			SetCapture(info._hwnd);
		}
		else if (userData.pSender == &SandEngine::Callback.OnMouseUp)
		{
			ReleaseCapture();
		}
		else if (userData.pSender == &SandEngine::Callback.OnMouseMove)
		{
			struct mouseMoveInfo
			{
				SVector2f pos;
				WPARAM param;
			};

			mouseMoveInfo& info = *(mouseMoveInfo*)userData.pUserData;
			if ((info.param & MK_RBUTTON) != 0)
			{
				SVector2f lastPos = SRenderer::Get().GetMainCamera().GetLastMousePressPos();
				SRenderer::Get().GetMainCamera().Rotate(info.pos.x - lastPos.x, info.pos.y - lastPos.y);
			}

			SRenderer::Get().GetMainCamera().SetLastMousePressPos(info.pos);
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
}