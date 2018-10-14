#include "imgui/imgui.h"
#include "IBL.h"
#include "EditorHelper.h"

void IBLEditor(char* envmapPath)
{
	ImGui::Text("Environment Map");
	ImGui::SameLine();
	ImGui::InputText("##Environment Map", envmapPath, IM_ARRAYSIZE(envmapPath));
	ImGui::SameLine();
	if (ImGui::Button("...##OpenEnviormentMap"))
	{
		std::string path = OpenFile("Environment Map\0*.hdr\0");
		strcpy_s(envmapPath, 1024, path.c_str());
	}

	if (ImGui::Button("Generate Irradiance Map"))
	{
		IBLGenerateIrradianceMap(envmapPath);
	}

	if (ImGui::Button("Generate BRDF Lut"))
	{
		IBLGenerateLutMap(envmapPath);
	}

	ImGui::SameLine();
	if (ImGui::Button("Generate Prefilter Map"))
	{
		IBLGeneratePrefilterMap(envmapPath);
	}
}