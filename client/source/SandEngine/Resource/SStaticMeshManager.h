#pragma once

#include "SandEngine/SApiDef.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandEngine/Resource/SMeshBuffer.h"

struct CustomSubMesh
{
	int iVertexStart = 0;
	int iVertexNum   = 0;
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
	std::string   m_Name;
};

class SStaticMesh : public SResource
{
public:
	SStaticMesh(const char* filename, class SResourceManager& manager);

public:
	SAND_API int GetSubmeshCount()        
	{ 
		return m_subMeshes.GetSize(); 
	}
	SMeshBuffer& GetMeshBuffer() 
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

private:
	SArray<CustomSubMesh>  m_subMeshes;
	SMeshBuffer            m_MeshBuffer;
	SArray<CustomMaterial> m_materials;
};

namespace SStaticMeshMangaer
{
	SAND_API SStaticMesh* LoadStaticMesh(const char* filename);
};