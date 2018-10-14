#pragma once

#include <windows.h>
#include <string>

static std::string OpenFile(const char* filter)
{
	char filename[MAX_PATH] = "";

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize  = sizeof(ofn);
	ofn.hwndOwner    = NULL;
	ofn.lpstrFile    = filename;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return GetOpenFileNameA(&ofn) ? filename : "";
}

static std::string SaveFile(const char* filter)
{
	char filename[MAX_PATH] = "";

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize  = sizeof(ofn);
	ofn.hwndOwner    = NULL;
	ofn.lpstrFile    = filename;
	ofn.nMaxFile     = MAX_PATH;
	ofn.lpstrFilter  = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags        = OFN_PATHMUSTEXIST;

	return GetSaveFileNameA(&ofn) ? filename : "";
}