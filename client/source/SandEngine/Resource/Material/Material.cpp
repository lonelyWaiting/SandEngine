#include "SandEnginePCH.h"
#include "Material.h"
#include <iostream>

extern "C"
{
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
}

static lua_State* g_pMatLuaState = nullptr;

int Print(lua_State* L)
{
	if (!lua_isstring(L, -1))
	{
		std::cout << "argument error, first argument isn't string" << std::endl;
		return 0;
	}

	std::cout << lua_tostring(L, -1) << std::endl;

	return 0;
}

int LoadFile(lua_State* L, const char* filename)
{
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "load file: " << filename << " failed" << std::endl;
		return 0;
	}

	return 0;
}

int DoFile(lua_State* L)
{
	if (!lua_isstring(L, -1))
	{
		std::cout << "argument error, first argument isn't string" << std::endl;
		return 0;
	}

	return LoadFile(L, lua_tostring(L, -1));
}

void sandMaterialLoad()
{
	// Enum Material Template File
}

void sandMaterialInit()
{
	sandMaterialDeInit();

	g_pMatLuaState = luaL_newstate();

	luaL_openlibs(g_pMatLuaState);

	lua_register(g_pMatLuaState, "MATPrint", Print);
	lua_register(g_pMatLuaState, "MATDoFile", DoFile);

	LoadFile(g_pMatLuaState, "materials/runtime/material_startup.lua");

	sandMaterialLoad();
}

void sandMaterialUpdate(float timeDiff)
{

}

void sandMaterialDeInit()
{
	if (g_pMatLuaState) lua_close(g_pMatLuaState);
}
