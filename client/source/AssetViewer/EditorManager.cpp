#include "EditorManager.h"
#include "SandEngine/Application/SandEngineModule.h"
#include "SandEngine/Callback/SCallback.h"
#include "SandEngine/GUI/SGui.h"
#include "imgui/imgui.h"

struct CustomEditorData
{
	char IBLEnvMapPath[1024];
}gEditorUserData;

static void CustomEditor(void*);

class GuiHandler : public SCallbackHandle
{
public:
	virtual void OnHandle( const SCallbackUserData& userData )
	{
		if( userData.pSender == &SandEngine::Callback.OnEngineInit )
		{
			GuiRegister(CustomEditor, &gEditorUserData);
		}
		else if( userData.pSender == &SandEngine::Callback.OnEngineDeInit )
		{
			GuiUnRegister(CustomEditor, &gEditorUserData);
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

void ImGuiDemo()
{
	static bool show_demo_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}

extern void IBLEditor(char* envmapPath);

void CustomEditor(void* userData)
{
	CustomEditorData& data = *(CustomEditorData*)userData;

	static bool enableImGuiDemo = false;
	static bool enableIBLDemo = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Demo"))
		{
			ImGui::MenuItem("IBL", "", &enableIBLDemo);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("ShowExample"))
		{
			ImGui::MenuItem("Enabled", "", &enableImGuiDemo);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (enableImGuiDemo)	ImGuiDemo();
	if (enableIBLDemo)		IBLEditor(data.IBLEnvMapPath);
}