#pragma once

bool ImGuiInit( void* hwnd );
void ImGuiShutdown();
void ImGuiBeginRender();
void ImGuiEndRender();
void ImGuiBeginResize();
void ImGuiEndResize();

#include "SandEngine/SApiDef.h"

SAND_API void GuiRegister( void* func , void* data = nullptr );
SAND_API void GuiUnRegister( void* func , void* data = nullptr );
void GuiUpdate();