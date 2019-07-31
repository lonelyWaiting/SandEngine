#pragma once

#include <vector>
#include <d3d11.h>
#include <map>
#include <string>
#include <set>

enum shaderStage
{
	SS_Vertex    = 0,
	SS_Pixel     = 1,
	SS_Hull      = 2,
	SS_Domain    = 3,
	SS_Compute   = 4,
	SS_Geometry  = 5,
	SS_StreamOut = 6,
	SS_Num       = SS_StreamOut + 1
};

enum variableType
{
	eVT_Unknow,
	eVT_FLOAT,
	eVT_FLOAT2,
	eVT_FLOAT3,
	eVT_FLOAT4,
	eVT_FLOAT4X4,
	eVT_INT,
	eVT_INT2,
	eVT_INT3,
	eVT_INT4,
	eVT_BOOL,
};

enum textureType
{
	eTT_Texture2D,
	eTT_Texture3D,
	eTT_TextureCube
};

enum filterMode
{
	eFM_Point,
	eFM_Linear,
	eFM_Trilinear
};

enum addressMode
{
	eAM_Wrap,
	eAM_Mirror,
	eAM_Clamp,
	eAM_Border,
	eAM_MirrorOnce
};

enum comparisonMode
{
	eCM_Never,
	eCM_Equal,
	eCM_Less,
	eCM_LessEqual,
	eCM_Greater,
	eCM_GreaterEqual,
	eCM_Always
};

struct shaderParameter
{
	std::string name;
	variableType type;

	bool operator < (const shaderParameter& parameter) const
	{
		return (name < parameter.name) || (type < parameter.type);
	}
};

struct constantBufferItem
{
	constantBufferItem()
	{
		parameter.name = "";
		parameter.type = eVT_FLOAT;
		offset        = 0;
		size          = 0;
		slot          = -1;
	}

	shaderParameter parameter;
	int			  offset;
	int			  size;
	int			  slot;
};

struct constantBufferInfo
{
	constantBufferInfo()
	{
		size = 0;
		for (int i = 0; i < SS_Num; i++)	bindStage[i] = false;
		for (int i = 0; i < SS_Num; i++)	slot[i] = -1;

		buffer = nullptr;
		data   = nullptr;
	}

	int			  size;
	int			  slot[SS_Num];
	bool		  bindStage[SS_Num];
	void*		  data;
	ID3D11Buffer* buffer;
	std::map<std::string, constantBufferItem> items;
};

struct textureInfo
{
	textureInfo()
	{
		for (int i = 0; i < SS_Num; i++)	slot[i] = -1;
		for (int i = 0; i < SS_Num; i++)	bindStage[i] = false;
		type = eTT_Texture2D;
	}
	int  slot[SS_Num];
	bool bindStage[SS_Num];
	textureType type;
};

struct samplerInfo
{
	samplerInfo()
	{
		for (int i = 0; i < SS_Num; i++)	slot[i] = -1;
		for (int i = 0; i < SS_Num; i++)	bindStage[i] = false;
		default_filter = eFM_Linear;
		default_address = eAM_Clamp;
		default_comp = eCM_Always;

		followTexture = false;
	}

	int slot[SS_Num];
	bool bindStage[SS_Num];
	bool followTexture;
	filterMode     default_filter;
	addressMode    default_address;
	comparisonMode default_comp;
	ID3D11SamplerState* samplerState;
};

struct shaderInputLayoutElem
{
	std::string semantic_name;
	int			semantic_index;
};

struct shaderPassPermutationInfo
{
	shaderPassPermutationInfo(const char* _name) { name = _name; }

	std::vector<std::string> keyword_list;

	std::string name;
	ID3D11VertexShader  * vertexShader   = nullptr;
	ID3D11PixelShader   * pixelShader    = nullptr;
	ID3D11HullShader    * hullShader     = nullptr;
	ID3D11DomainShader  * domainShader   = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;

	std::map<std::string, constantBufferInfo> cbList;
	std::map<std::string, textureInfo>		  textureList;
	std::map<std::string, samplerInfo>		  samplerList;
	std::vector<shaderInputLayoutElem>		  inputLayoutElem;

	// todo: support type check, element type should be std::string + variableType
	std::set<shaderParameter> parameterSet;
};

struct shaderPassInfo
{
	shaderPassInfo(const char* _name) { name = _name; }
	std::string name;
	std::vector<shaderPassPermutationInfo> permutation_list;
};

struct shaderInfo
{
	std::string name;
	std::vector<shaderPassInfo> pass_list;
};

extern std::map<std::string, shaderInfo> g_shaderMap;
