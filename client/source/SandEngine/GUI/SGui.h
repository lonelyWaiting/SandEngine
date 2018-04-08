#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

bool ImGuiInit( void* hwnd , ID3D11Device* device , ID3D11DeviceContext* device_context );
void ImGuiShutdown();
void ImGuiBeginRender();
void ImGuiEndRender();
void ImGuiBeginResize();
void ImGuiEndResize();