#include "SandEnginePCH.h"
#include "Material.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Math/SVector4f.h"
#include "SandBase/Math/SMatrix4f.h"
#include "Resource/Texture/STextureObject.h"
#include "Resource/Shaders/SShader.h"
#include "SandBase/Log/SLog.h"
#include "Pipeline/SRenderHelper.h"
#include "Resource/Texture/STextureManager.h"

SMaterial::SMaterial(const char* filename)
{
	shader = SShader::FindOrAdd(filename);
}

void SMaterial::SetFloat(const char * name, float v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_FLOAT;
	paramList[param].vf = v;
}

void SMaterial::SetFloat2(const char * name, SVector2f v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_FLOAT2;
	paramList[param].vf2 = v;
}

void SMaterial::SetFloat3(const char * name, SVector3f v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_FLOAT3;
	paramList[param].vf3 = v;
}

void SMaterial::SetFloat4(const char * name, SVector4f v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_FLOAT4;
	paramList[param].vf4 = v;
}

void SMaterial::SetInt(const char * name, int v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_INT;
	paramList[param].vi = v;
}

void SMaterial::SetBool(const char * name, bool v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_BOOL;
	paramList[param].vb = v;
}

void SMaterial::SetMatrix(const char * name, SMatrix4f v)
{
	shaderParameter param;
	param.name = name;
	param.type = eVT_FLOAT4X4;
	paramList[param].vm = v;
}

void SMaterial::SetTexture(const char * name, STexture2D* tex)
{
	textureList[name].texture = tex;
}

void SMaterial::SetTextureOffset(const char * name, SVector2f offset)
{
	textureList[name].offset = offset;
}

void SMaterial::SetTextureScale(const char * name, SVector2f scale)
{
	textureList[name].scale = scale;
}

void SMaterial::SetPass(int pass)
{
	if (pass < 0 || pass >= shader->GetPassCount())
	{
		SLog::Error("Shader Pass Index Invalid.");
		return;
	}
	curPass = pass;
}

void SMaterial::Apply()
{
	int permutationIndex = ValidateParam();
	if (permutationIndex == -1)	return;

	auto pass = shader->GetPass(curPass);
	auto permutation = pass.permutation_list[permutationIndex];

	// bind texture
	for (auto iter = permutation.textureList.begin(); iter != permutation.textureList.end(); iter++)
	{
		auto tex = textureList.find(iter->first.c_str());

		// default texture using black
		STexture2D* tex2D = tex != textureList.end() ? tex->second.texture : STextureManager::GetBlackTexture();

		if (iter->second.bindStage[SS_Vertex])
		{
			SRenderHelper::BindTexture(eST_Vertex , iter->second.slot[eST_Vertex], tex2D);
		}

		if (iter->second.bindStage[SS_Pixel])
		{
			SRenderHelper::BindTexture(eST_Pixel  , iter->second.slot[eST_Pixel], tex2D);
		}

		if (iter->second.bindStage[SS_Domain])
		{
			SRenderHelper::BindTexture(eST_Domain , iter->second.slot[eST_Domain], tex2D);
		}

		if (iter->second.bindStage[SS_Hull])
		{
			SRenderHelper::BindTexture(eST_Hull   , iter->second.slot[eST_Hull], tex2D);
		}

		if (iter->second.bindStage[SS_Geometry])
		{
			SRenderHelper::BindTexture(eST_Geometry, iter->second.slot[SS_Geometry], tex2D);
		}

		if (iter->second.bindStage[SS_Compute])
		{
			SRenderHelper::BindTexture(eST_Compute, iter->second.slot[SS_Compute], tex2D);
		}
	}

	// bind sampler
	for (auto iter = permutation.samplerList.begin(); iter != permutation.samplerList.end(); iter++)
	{
		if (!iter->second.followTexture)
		{
			if (iter->second.bindStage[SS_Vertex])
			{
				SRenderHelper::g_ImmediateContext->VSSetSamplers(iter->second.slot[SS_Vertex] , 1 , &iter->second.samplerState);
			}

			if (iter->second.bindStage[SS_Pixel])
			{
				SRenderHelper::g_ImmediateContext->PSSetSamplers(iter->second.slot[SS_Pixel] , 1 , &iter->second.samplerState);
			}

			if (iter->second.bindStage[eST_Domain])
			{
				SRenderHelper::g_ImmediateContext->DSSetSamplers(iter->second.slot[SS_Domain] , 1 , &iter->second.samplerState);
			}

			if (iter->second.bindStage[eST_Hull])
			{
				SRenderHelper::g_ImmediateContext->HSSetSamplers(iter->second.slot[SS_Hull] , 1 , &iter->second.samplerState);
			}

			if (iter->second.bindStage[eST_Geometry])
			{
				SRenderHelper::g_ImmediateContext->GSSetSamplers(iter->second.slot[SS_Geometry] , 1 , &iter->second.samplerState);
			}

			if (iter->second.bindStage[eST_Compute])
			{
				SRenderHelper::g_ImmediateContext->CSSetSamplers(iter->second.slot[SS_Compute] , 1 , &iter->second.samplerState);
			}
		}
	}

	// bind constant buffer
	for (auto iter = permutation.cbList.begin(); iter != permutation.cbList.end(); iter++)
	{
		bool cbNeedUpdate = false;

		auto& items = iter->second.items;
		for (auto item = items.begin(); item != items.end(); item++)
		{
			auto param = paramList.find(item->second.parameter);
			if (param != paramList.end())
			{
				memcpy((char*)iter->second.data + item->second.offset , &param->second , item->second.size);
				cbNeedUpdate = true;
			}
		}

		if (cbNeedUpdate)
		{
			D3D11_MAPPED_SUBRESOURCE dataPtr;
			SRenderHelper::g_ImmediateContext->Map(iter->second.buffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &dataPtr);
			memcpy(dataPtr.pData , iter->second.data , iter->second.size);
			SRenderHelper::g_ImmediateContext->Unmap(iter->second.buffer , 0);
		}
	}

	// bind shader
	if (permutation.vertexShader   != nullptr)	SRenderHelper::g_ImmediateContext->VSSetShader(permutation.vertexShader  , nullptr, 0);
	if (permutation.pixelShader    != nullptr)	SRenderHelper::g_ImmediateContext->PSSetShader(permutation.pixelShader   , nullptr, 0);
	if (permutation.geometryShader != nullptr)	SRenderHelper::g_ImmediateContext->GSSetShader(permutation.geometryShader, nullptr, 0);
	if (permutation.hullShader     != nullptr)	SRenderHelper::g_ImmediateContext->HSSetShader(permutation.hullShader    , nullptr, 0);
	if (permutation.domainShader   != nullptr)	SRenderHelper::g_ImmediateContext->DSSetShader(permutation.domainShader  , nullptr, 0);
}

void SMaterial::EnableKeyword(const char * keyword)
{
	int index = keywordList.Find(keyword);
	if (index == -1)
	{
		keywordList.PushBack(keyword);
		keywordList.Sort();
	}
}

void SMaterial::DisableKeyword(const char * keyword)
{
	keywordList.RemoveValue(keyword);
}

int SMaterial::ValidateParam()
{
	if (curPass < 0 || curPass >= shader->GetPassCount())	return -1;

	const shaderPassInfo& passInfo = shader->GetPass(curPass);
	
	int permutationIndex = -1;
	for (int i = 0 , count = passInfo.permutation_list.size(); i < count; i++)
	{
		const auto& srcKeywordList = passInfo.permutation_list[i].keyword_list;
		if (srcKeywordList.size() == keywordList.GetSize())
		{
			permutationIndex = i;
			for (int j = 0 , n = keywordList.GetSize(); j < n; j++)
			{
				if (!keywordList[j].IsEqual(srcKeywordList[j].c_str()))
				{
					permutationIndex = -1;
					break;
				}
			}

			if (permutationIndex != -1)	break;
		}
	}

	if (permutationIndex == -1)
	{
		SLog::Error("can't find match keyword list");
		return -1;
	}

	const auto& permutationInfo = passInfo.permutation_list[permutationIndex];

	// validate constant buffer element
	for (auto iter = paramList.begin(); iter != paramList.end();)
	{
		if (permutationInfo.parameterSet.find(iter->first) == permutationInfo.parameterSet.end())
		{
			iter = paramList.erase(iter);
			continue;
		}

		iter++;
	}

	return permutationIndex;
}
