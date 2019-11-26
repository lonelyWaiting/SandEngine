#include <vector>
#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "shader_parser.h"
#include "shader.h"

extern "C"
{
	#include "Lua/Inc/lua.h"
	#include "Lua/Inc/lauxlib.h"
	#include "Lua/Inc/lualib.h"
}

std::map<std::string, shaderInfo> g_shaderMap;

class SShaderInclude : public ID3DInclude
{
public:
	SShaderInclude(const char* InShaderDir, const char* InSystemDir)
	{
		shaderDir = InShaderDir;
		systemDir = InSystemDir;
	}

	HRESULT Open(D3D_INCLUDE_TYPE IncludeType ,
				 LPCSTR           pFileName ,
				 LPCVOID          pParentData ,
				 LPCVOID          *ppData ,
				 UINT             *pBytes)
	{
		std::string finalPath;
		switch (IncludeType)
		{
		case D3D_INCLUDE_LOCAL:
			finalPath = shaderDir + "\\" + pFileName;
			break;

		case D3D_INCLUDE_SYSTEM:
			finalPath = systemDir + "\\" + pFileName;
			break;
		}

		// open file
		std::ifstream includeFile(finalPath.c_str() , std::ios::in | std::ios::binary | std::ios::ate);

		if (includeFile.is_open())
		{
			long long fileSize = includeFile.tellg();
			char* buf = new char[fileSize];
			includeFile.seekg(0 , std::ios::beg);
			includeFile.read(buf , fileSize);
			includeFile.close();
			*ppData = buf;
			*pBytes = fileSize;
		}
	}

	HRESULT Close(LPCVOID pData)
	{
		char* buf = (char*)pData;
		delete[] buf;
		buf = nullptr;
		return S_OK;
	}

public:
	std::string shaderDir;
	std::string systemDir;
};

static SShaderInclude* IncludeHandler = nullptr;

shaderPassPermutationInfo& shader_permutation_create(const char* shader_name, const char* pass_name, const std::vector<std::string>& keyword_list)
{
	auto iter = g_shaderMap.find(shader_name);
	if (iter == g_shaderMap.end()) { g_shaderMap[shader_name].name = shader_name; }

	auto& cur_shader = g_shaderMap[shader_name];

	// add one if it didn't existed or return the one finded
	int index = -1;
	for (int pass_index = 0; pass_index < (int)cur_shader.pass_list.size(); pass_index++)
	{
		if (strcmp(pass_name, cur_shader.pass_list[pass_index].name.c_str()) == 0)
		{
			index = pass_index;
			break;
		}
	}

	if (index == -1)
	{
		cur_shader.pass_list.push_back(shaderPassInfo(pass_name));
		index = (unsigned int)cur_shader.pass_list.size() - 1;
	}

	// did pass name exsited yet
	auto& pass      = cur_shader.pass_list[index];
	auto& permutation_list = pass.permutation_list;
	auto pass_index = -1;
	for (int i = 0; i < (int)permutation_list.size(); i++)
	{
		if (permutation_list[i].name == pass_name)
		{
			pass_index = i;
			continue;
		}
	}

	if (pass_index == -1)
	{
		permutation_list.push_back(shaderPassPermutationInfo(pass_name));
		pass_index = (int)permutation_list.size() - 1;
	}

	auto& permutation = permutation_list[pass_index];
	permutation.keyword_list = keyword_list;

	return permutation;
}

static std::map<std::string, variableType> variable_type_map;

void shader_init_variable_type_map()
{
	variable_type_map["float"]  = eVT_FLOAT;
	variable_type_map["float2"] = eVT_FLOAT2;
	variable_type_map["float3"] = eVT_FLOAT3;
	variable_type_map["float4"] = eVT_FLOAT4;

	variable_type_map["int"]  = eVT_INT;
	variable_type_map["int2"] = eVT_INT2;
	variable_type_map["int3"] = eVT_INT3;
	variable_type_map["int4"] = eVT_INT4;

	variable_type_map["bool"] = eVT_BOOL;

	variable_type_map["float4x4"] = eVT_FLOAT4X4;
}

variableType shader_map_variable_type(const char* type_name)
{
	auto iter = variable_type_map.find(type_name);
	if (iter == variable_type_map.end()) { std::cout << "unsupported variable type" << std::endl; return eVT_Unknow; }
	return iter->second;
}

//class SShaderInclude : public ID3DInclude
//{
//public:
//	HRESULT _stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
//	{
//		std::string str;
//		for (unsigned int i = 0; i < (unsigned int)searchPath.size(); i++)
//		{
//			str = searchPath[i];
//			str += pFileName;
//
//			std::ifstream fi;
//			fi.open(str.c_str(), std::ios::in | std::ios::binary);
//			if (!fi.is_open())	continue;
//
//			int curPos = (int)fi.tellg();
//			fi.seekg(0, std::ios::end);
//			size = (int)fi.tellg();
//			fi.seekg(curPos);
//
//			pData = new char[size];
//			fi.read(pData, size);
//
//			fi.close();
//
//			break;
//		}
//
//		if (pData) { *ppData = pData; *pBytes = size; }
//		return S_OK;
//	}
//
//	HRESULT _stdcall Close(LPCVOID pData)
//	{
//		if (pData != nullptr) { delete[] pData; pData = nullptr; }
//		size = 0;
//		return S_OK;
//	}
//public:
//	std::vector<std::string> searchPath;
//	char* pData = nullptr;
//	unsigned int size = 0;
//}include_handler;

static lua_State*	 g_pMatLuaState = nullptr;
static ID3D11Device* g_pDevice      = nullptr;

static int LoadFile(lua_State* L, const char* filename)
{
    std::vector<char> data;
    std::ifstream fi;
    fi.open(filename, std::ios::in | std::ios::binary);
	if (!fi.is_open())
	{
        std::cout << "file: " << filename << " open failed" << std::endl;
		return  0;
	}

    int curPos = (int)fi.tellg();
    fi.seekg(0, std::ios::end);
    int size = (int)fi.tellg();
    fi.seekg(curPos);

    data.resize(size);
    fi.read(&data[0], size);
    fi.close();

    if (luaL_loadbuffer(L, &data[0], data.size(), filename) || lua_pcall(L, 0, 0, 0))
    {
        std::cout << "load file: " << filename << " failed" << std::endl;
        return 0;
    }

    return 0;
}

static int DoFile(lua_State* L)
{
    if (!lua_isstring(L, -1))
    {
        std::cout << "argument error, first argument isn't string" << std::endl;
        return 0;
    }

    return LoadFile(L, lua_tostring(L, -1));
}

static int DoString(lua_State* L)
{
    const char* data = lua_tostring(L, -1);

    if (luaL_dostring(L, data))
    {
        std::cout << "do string failed" << std::endl << data << std::endl;
        return 0;
    }

    return 0;
}

void shader_cleanup()
{
    if (g_pMatLuaState) lua_close(g_pMatLuaState);
}

samplerInfo shader_parse_sampler(const char* name)
{
	lua_getglobal(g_pMatLuaState, "parse_sampler");
	lua_pushstring(g_pMatLuaState, name);
	if (lua_pcall(g_pMatLuaState, 1, 1, 0) != LUA_OK)
	{
		std::cout << "execute read_shader_file failed" << std::endl;
		return samplerInfo();
	}

	samplerInfo info;

	lua_pushnil(g_pMatLuaState);
	while (lua_next(g_pMatLuaState, -2) != 0)
	{
		// key at -2, value at -1
		const char* key = lua_tostring(g_pMatLuaState, -2);

		if (strcmp(key, "filter") == 0)
		{
			info.default_filter = (filterMode)lua_tointeger(g_pMatLuaState, -1);
		}
		else if (strcmp(key, "address") == 0)
		{
			info.default_address = (addressMode)lua_tointeger(g_pMatLuaState, -1);
		}
		else if (strcmp(key, "comparison") == 0)
		{
			info.default_comp = (comparisonMode)lua_tointeger(g_pMatLuaState, -1);
		}
		else if (strcmp(key, "follow_texture") == 0)
		{
			info.followTexture = lua_toboolean(g_pMatLuaState, -1) != 0;
		}
		lua_pop(g_pMatLuaState, 1);
	}

	return info;
}

void CreateSampler(samplerInfo& info)
{
	D3D11_SAMPLER_DESC Desc;
	ZeroMemory(&Desc , sizeof(Desc));

	switch (info.default_address)
	{
	case eAM_Clamp:
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;

	case eAM_Border:
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		break;

	case eAM_Mirror:
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;

	case eAM_MirrorOnce:
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		break;

	case eAM_Wrap:
		Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	}

	switch (info.default_filter)
	{
	case eFM_Linear:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;

	case eFM_Point:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;

	case eFM_Trilinear:
		Desc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	switch (info.default_comp)
	{
	case eCM_Always:
		Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		break;

	case eCM_Equal:
		Desc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
		break;

	case eCM_Greater:
		Desc.ComparisonFunc = D3D11_COMPARISON_GREATER;
		break;

	case eCM_GreaterEqual:
		Desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;

	case eCM_Less:
		Desc.ComparisonFunc = D3D11_COMPARISON_LESS;
		break;

	case eCM_LessEqual:
		Desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;

	case eCM_Never:
		Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		break;
	}

	g_pDevice->CreateSamplerState(&Desc , &info.samplerState);
}

static void shader_reflect(shaderPassPermutationInfo& permutation, ID3DBlob* blob, shaderStage type)
{
	if (!blob) return;

    ID3D11ShaderReflection* pReflector = nullptr;
	HRESULT result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
    if (FAILED(result)) { std::cout << "Failed to create shader reflection interface!" << std::endl; return; }

    D3D11_SHADER_DESC desc;
	pReflector->GetDesc(&desc);
	
	// init vertex shader's input layout
	if (type == SS_Vertex)
	{
		for (int i = 0; i < (int)desc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC inputSignDesc;
			pReflector->GetInputParameterDesc(i, &inputSignDesc);

			if (inputSignDesc.SystemValueType == D3D_NAME_UNDEFINED)
			{
				shaderInputLayoutElem inputLayout;
				inputLayout.semantic_name  = inputSignDesc.SemanticName;
				inputLayout.semantic_index = inputSignDesc.SemanticIndex;
				permutation.inputLayoutElem.push_back(inputLayout);
			}
		}
	}

	// global constant buffer list initialzation
	for (int i = 0; i < (int)desc.ConstantBuffers; i++)
    {
		ID3D11ShaderReflectionConstantBuffer* pConstantBuffer = pReflector->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bufferDesc;
		pConstantBuffer->GetDesc(&bufferDesc);

		// update bindStage of existed constant Buffer in list
		auto iter = permutation.cbList.find(bufferDesc.Name);
		if (iter != permutation.cbList.end())
		{
			iter->second.bindStage[type] = true;
			continue;
		}

		if (bufferDesc.Type == D3D_CT_CBUFFER || bufferDesc.Type == D3D_CT_TBUFFER)
		{
			constantBufferInfo cb;
			cb.size            = bufferDesc.Size;
			cb.bindStage[type] = true;

			for (int j = 0; j < (int)bufferDesc.Variables; j++)
			{
				constantBufferItem mapping;

				ID3D11ShaderReflectionVariable* pVariable = pConstantBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC var_desc;
				pVariable->GetDesc(&var_desc);

				ID3D11ShaderReflectionType* pType = pVariable->GetType();
				D3D11_SHADER_TYPE_DESC typeDesc;
				pType->GetDesc(&typeDesc);

				mapping.parameter.name = var_desc.Name;
				mapping.offset         = var_desc.StartOffset;
				mapping.size           = var_desc.Size;
				mapping.parameter.type = shader_map_variable_type(typeDesc.Name);

				cb.items[var_desc.Name] = mapping;
			}

			permutation.cbList[bufferDesc.Name] = cb;
		}
    }

	// dx constant buffer and binded data intialization
	auto& cb_list = permutation.cbList;
	auto  cb_iter = cb_list.begin();
	while (cb_iter != cb_list.end())
	{
		auto& cb = cb_iter->second;

		// value in it could be dynamic updated
		cb.data = malloc(cb.size);

		D3D11_BUFFER_DESC cb_desc;
		ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));
		cb_desc.ByteWidth      = cb.size;
		cb_desc.Usage          = D3D11_USAGE_DYNAMIC;
		cb_desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT result = g_pDevice->CreateBuffer(&cb_desc, nullptr, &cb.buffer);
		if (FAILED(result))
		{
			std::cout << "create constant buffer " << cb_iter->first.c_str() << " failed" << std::endl;
			free(cb.data);
			cb.data   = nullptr;
			cb.buffer = nullptr;
		}

		cb_iter++;
	}

	// shader bound resouce
	for (unsigned int i = 0; i < desc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC resource_bind_desc;
		pReflector->GetResourceBindingDesc(i, &resource_bind_desc);

		switch (resource_bind_desc.Type)
		{
			case D3D_SIT_CBUFFER:
			{
				auto iter = permutation.cbList.find(resource_bind_desc.Name);
				if (iter == permutation.cbList.end())	continue;
				iter->second.slot[type] = resource_bind_desc.BindPoint;
				break;
			}

			case D3D_SIT_TEXTURE:
			{
				auto iter = permutation.textureList.find(resource_bind_desc.Name);
				if (iter != permutation.textureList.end())
				{
					iter->second.bindStage[type] = true;
					iter->second.slot[type]      = resource_bind_desc.BindPoint;
				}
				else
				{
					textureInfo info;
					resource_bind_desc.Name;
					info.slot[type]      = resource_bind_desc.BindPoint;
					info.bindStage[type] = true;
					switch (resource_bind_desc.Dimension)
					{
						case D3D_SRV_DIMENSION_TEXTURE2D:
							info.type = eTT_Texture2D;
							break;

						case D3D_SRV_DIMENSION_TEXTURE3D:
							info.type = eTT_Texture3D;
							break;

						case D3D_SRV_DIMENSION_TEXTURECUBE:
							info.type = eTT_TextureCube;
							break;
					}
					permutation.textureList[resource_bind_desc.Name] = info;
				}

				break;
			}

			case D3D_SIT_SAMPLER:
			{
				auto iter = permutation.samplerList.find(resource_bind_desc.Name);
				if (iter != permutation.samplerList.end())
				{
					iter->second.slot[type]      = resource_bind_desc.BindPoint;
					iter->second.bindStage[type] = true;
				}
				else
				{
					samplerInfo info = shader_parse_sampler(resource_bind_desc.Name);
					info.slot[type]      = resource_bind_desc.BindPoint;
					info.bindStage[type] = true;
					CreateSampler(info);
					permutation.samplerList[resource_bind_desc.Name] = info;
				}
				
				break;
			}

			default:
				std::cout << "unsupported resource type" << std::endl;
				break;
		}
	}
}

static int shader_compile(lua_State* L)
{
	if (g_pDevice == nullptr)	return 0;

    const char* shader_name        = lua_tostring(L, 1);
    const char* pass_name          = lua_tostring(L, 2);
    const char* debug_name         = lua_tostring(L, 3);
    const char* shader_source_text = lua_tostring(L, 4);
    const char* vertex_entry       = lua_tostring(L, 5);
    const char* fragment_entry     = lua_tostring(L, 6);
	const char* hull_entry         = lua_tostring(L, 7);
	const char* domain_entry       = lua_tostring(L, 8);
	const char* geometry_entry     = lua_tostring(L, 9);

	std::vector<std::string> keyword_list;

	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		// key at -2, value at -1
		const char* value = lua_tostring(L, -1);
		keyword_list.push_back(value);
		lua_pop(L, 1);
	}

	shaderPassPermutationInfo& permutation = shader_permutation_create(shader_name, pass_name, keyword_list);
	ID3DBlob* errorBlob = nullptr;

	// -----------------------------------------Vertex Shader-----------------------------------------
	ID3D11VertexShader* pVertexShader = nullptr;
    ID3DBlob* vertexBlob = nullptr;
	if (strcmp(vertex_entry, "") != 0)
	{
		HRESULT result = D3DCompile(shader_source_text ,
									strlen(shader_source_text) ,
									debug_name ,
									nullptr ,
									IncludeHandler ,
									vertex_entry ,
									"vs_5_0" ,
									0 ,
									0 ,
									&vertexBlob ,
									&errorBlob);
		if (FAILED(result))
		{
			if (errorBlob)	std::cout << "vertex shader compile failed: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			return 0;
		}
		g_pDevice->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &pVertexShader);
	}
	permutation.vertexShader = pVertexShader;

	// -----------------------------------------Pixel Shader-----------------------------------------
	ID3D11PixelShader* pPixelShader = nullptr;
    ID3DBlob* pixelBlob = nullptr;
	if (strcmp(fragment_entry, "") != 0)
	{
		HRESULT result = D3DCompile(shader_source_text,
									strlen(shader_source_text),
									debug_name,
									nullptr,
									IncludeHandler ,
									fragment_entry,
									"ps_5_0",
									0,
									0,
									&pixelBlob,
									&errorBlob);
		if (FAILED(result))
		{
			if(errorBlob)	std::cout << "pixel shader compile failed: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			return 0;
		}
		g_pDevice->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &pPixelShader);
	}
	permutation.pixelShader = pPixelShader;
	
	// -----------------------------------------Hull Shader-----------------------------------------
	ID3D11HullShader* pHullShader = nullptr;
	ID3DBlob* hullBlob = nullptr;
	if (strcmp(hull_entry, "") != 0)
	{
		HRESULT result = D3DCompile(shader_source_text,
									strlen(shader_source_text),
									debug_name,
									nullptr,
									IncludeHandler ,
									hull_entry,
									"hs_5_0",
									0,
									0,
									&hullBlob,
									&errorBlob);
		if (FAILED(result))
		{
			if (errorBlob)	std::cout << "hull shader compile failed: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			return 0;
		}
		g_pDevice->CreateHullShader(hullBlob->GetBufferPointer(), hullBlob->GetBufferSize(), nullptr, &pHullShader);
	}
	permutation.hullShader = pHullShader;

	// -----------------------------------------Domain Shader-----------------------------------------
	ID3D11DomainShader* pDomainShader = nullptr;
	ID3DBlob* domainBlob = nullptr;
	if (strcmp(domain_entry, "") != 0)
	{
		HRESULT result = D3DCompile(shader_source_text,
									strlen(shader_source_text),
									debug_name,
									nullptr,
									IncludeHandler ,
									domain_entry,
									"ds_5_0",
									0,
									0,
									&domainBlob,
									&errorBlob);

		if (FAILED(result))
		{
			if (errorBlob)	std::cout << "domain shader compile failed: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			return 0;
		}
		g_pDevice->CreateDomainShader(domainBlob->GetBufferPointer(), domainBlob->GetBufferSize(), nullptr, &pDomainShader);
	}
	permutation.domainShader = pDomainShader;

	// -----------------------------------------Geometry Shader-----------------------------------------
	ID3D11GeometryShader* pGeometryShader = nullptr;
	ID3DBlob* geometryBlob = nullptr;
	if (strcmp(geometry_entry, "") != 0)
	{
		HRESULT result = D3DCompile(shader_source_text,
									strlen(shader_source_text),
									debug_name,
									nullptr,
									IncludeHandler ,
									geometry_entry,
									"gs_5_0",
									0,
									0,
									&geometryBlob,
									&errorBlob);
		if (FAILED(result))
		{
			if(errorBlob)	std::cout << "geometry shader compile failed: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			return 0;
		}
		g_pDevice->CreateGeometryShader(geometryBlob->GetBufferPointer(), geometryBlob->GetBufferSize(), nullptr, &pGeometryShader);
	}
	permutation.geometryShader = pGeometryShader;

	shader_reflect(permutation, vertexBlob	 , SS_Vertex);
	shader_reflect(permutation, pixelBlob	 , SS_Pixel);
	shader_reflect(permutation, hullBlob	 , SS_Hull);
	shader_reflect(permutation, domainBlob	 , SS_Domain);
	shader_reflect(permutation, geometryBlob, SS_Geometry);

    return 0;
}

static int shader_print(lua_State* L)
{
    if (!lua_isstring(L, -1))
    {
        std::cout << "argument error, first argument isn't string" << std::endl;
        return 0;
    }

    std::cout << lua_tostring(L, -1) << std::endl;

    return 0;
}

static int shader_finish_compile(lua_State* L)
{
	const char* shader_name = lua_tostring(L , -1);

	auto iter = g_shaderMap.find(shader_name);
	if (iter == g_shaderMap.end())	return  0;

	auto& passList = iter->second.pass_list;
	for (int i = 0 , pass_count = (int) passList.size(); i < pass_count; i++)
	{
		auto& pass = passList[i];

		for (int j = 0 , permutation_count = (int) pass.permutation_list.size(); j < permutation_count; j++)
		{
			auto& permutation = pass.permutation_list[j];
			permutation.parameterSet.clear();

			for (auto it = permutation.cbList.begin(); it != permutation.cbList.end(); it++)
			{
				auto& items = it->second.items;
				
				for (auto xx = items.begin(); xx != items.end(); xx++)
				{
					permutation.parameterSet.insert(xx->second.parameter);
				}
			}
		}
	}

	return 0;
}

void shader_init(ID3D11Device* device, const char* shader_parser_search_path)
{
	g_pDevice = device;

    shader_cleanup();
    g_pMatLuaState = luaL_newstate();
    luaL_openlibs(g_pMatLuaState);

    lua_register(g_pMatLuaState, "compile", shader_compile);
    lua_register(g_pMatLuaState, "print"  , shader_print);
	lua_register(g_pMatLuaState , "finishCompile" , shader_finish_compile);
	shader_init_variable_type_map();

	std::string shader_define_path    = std::string(shader_parser_search_path) + "shader_define.lua";
	std::string shader_generator_path = std::string(shader_parser_search_path) + "shader_generator.lua";
	LoadFile(g_pMatLuaState , shader_generator_path.c_str());
	LoadFile(g_pMatLuaState , shader_define_path.c_str());

	IncludeHandler = new SShaderInclude("" , "");
}

bool shader_load(const char* filename)
{
    lua_getglobal(g_pMatLuaState, "parse_shader");
    lua_pushstring(g_pMatLuaState, filename);
	if (lua_pcall(g_pMatLuaState, 1, 1, 0) != LUA_OK)
	{
		std::cout << "parse shader " << filename << " failed"  << std::endl;
		return false;
	}

	return true;
} 

void shader_add_search_path(const char* path)
{
	//include_handler.searchPath.push_back(path);
}
