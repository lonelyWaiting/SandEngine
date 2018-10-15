#pragma once

#include "SandEngine/SApiDef.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandEngine/Resource/SMeshBuffer.h"

struct CustomSubMesh
{
	int iVertexStart = 0;
	int iIndexStart  = 0;
	int iIndexNum    = 0;
	int iMatID       = -1;
};

struct CustomMaterial
{
	STexture2DPtr m_DiffuseMap;
	STexture2DPtr m_NormalMap;
	STexture2DPtr m_MetallicMap;
	STexture2DPtr m_roughnessMap;
	STexture2DPtr m_AOMap;
	std::string   m_Name;
};

class SStaticMesh : public SResource
{
public:
	SStaticMesh(const char* filename, class SResourceManager& manager);

public:
	SAND_API int GetSubmeshCount() const
	{ 
		return m_subMeshes.GetSize(); 
	}
	SMeshBuffer& GetMeshBuffer() 
	{ 
		return m_MeshBuffer; 
	}
	const SMeshBuffer& GetMeshBuffer() const
	{
		return m_MeshBuffer;
	}
	CustomSubMesh& AppendSubmesh() 
	{ 
		m_subMeshes.PushBack(CustomSubMesh()); return m_subMeshes.Peek(); 
	}
	void SetMaterials(SArray<CustomMaterial>& materials)
	{
		m_materials.Swap(materials);
	}
	int GetSubmeshIndexCount(int i) const
	{
		return m_subMeshes[i].iIndexNum;
	}
	int GetSubmeshStartIndex(int i) const
	{
		return m_subMeshes[i].iIndexStart;
	}
	int GetSubmeshVertexOffset(int i) const
	{
		return m_subMeshes[i].iVertexStart;
	}

	STexture2D* GetDiffuseMap(int i)
	{
		if (m_subMeshes[i].iMatID < 0 || m_subMeshes[i].iMatID >= (int)m_materials.GetSize())	return nullptr;

		return m_materials[m_subMeshes[i].iMatID].m_DiffuseMap.GetPointer();
	}

	STexture2D* GetMetallicMap(int i)
	{
		if (m_subMeshes[i].iMatID < 0 || m_subMeshes[i].iMatID >= (int)m_materials.GetSize())	return nullptr;

		return m_materials[m_subMeshes[i].iMatID].m_MetallicMap.GetPointer();
	}

	STexture2D* GetNormalMap(int i)
	{
		if (m_subMeshes[i].iMatID < 0 || m_subMeshes[i].iMatID >= (int)m_materials.GetSize())	return nullptr;

		return m_materials[m_subMeshes[i].iMatID].m_NormalMap.GetPointer();
	}

	STexture2D* GetRoughnessMap(int i)
	{
		if (m_subMeshes[i].iMatID < 0 || m_subMeshes[i].iMatID >= (int)m_materials.GetSize())	return nullptr;

		return m_materials[m_subMeshes[i].iMatID].m_roughnessMap.GetPointer();
	}

	STexture2D* GetAOMap(int i)
	{
		if (m_subMeshes[i].iMatID < 0 || m_subMeshes[i].iMatID >= (int)m_materials.GetSize())	return nullptr;

		return m_materials[m_subMeshes[i].iMatID].m_AOMap.GetPointer();
	}
private:
	SArray<CustomSubMesh>  m_subMeshes;
	SMeshBuffer            m_MeshBuffer;
	SArray<CustomMaterial> m_materials;
};

namespace SStaticMeshMangaer
{
	SAND_API SStaticMesh* LoadStaticMesh(const char* filename);
};