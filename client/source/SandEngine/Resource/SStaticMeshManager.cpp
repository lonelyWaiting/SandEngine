#include "SandEnginePCH.h"
#include "SStaticMeshManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SResourceManager.h"
#include "SandBase/Math/SVector2f.h"
#include "SandBase/Math/SVector3f.h"
#include "SandBase/Vector/SArray.h"
#include "SandEngine/Pipeline/SRenderHelper.h"
#include "SandBase/Log/SLog.h"
#include "SandEngine/Resource/Texture/STextureManager.h"
#include "SandEngine/Resource/Texture/STextureObject.h"

SResourceManager g_StaticMeshManager;

SStaticMesh::SStaticMesh(const char* filename, class SResourceManager& manager)
	:SResource(filename, manager)
{

}

struct CustomVertex
{
	SVector3f position;
	SVector3f Normal;
	SVector3f Tangent;
	SVector2f texcoord;
};

struct CustomTexture
{
	std::string type;
	std::string path;
	STexture2DPtr texture;
};

class Mesh
{
public:
	SArray<CustomVertex>  m_vertices;
	SArray<CustomTexture> m_textures;
	SArray<unsigned int>  m_indices;
	int					  m_matIndex = -1;

public:
	Mesh() {}

	void SetVertices(SArray<CustomVertex>& vertices)
	{
		m_vertices.Swap(vertices);
	}

	void SetIndices(SArray<unsigned int>& indices)
	{
		m_indices.Swap(indices);
	}

	void SetTextures(SArray<CustomTexture>& textures)
	{
		m_textures.Swap(textures);
	}

	void SetMaterialIndex(int matIndex)
	{
		m_matIndex = matIndex;
	}
};

void ProcessNode(const aiNode* node, const aiScene* scene, SArray<Mesh>& meshes, SArray<CustomTexture>& LoadedTextures, std::string& directory)
{
	meshes.Reserve(meshes.GetSize() + node->mNumMeshes);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.PushBack(Mesh());

		SArray<CustomVertex > vertices;
		SArray<CustomTexture> textures;
		SArray<unsigned int > indices;

		// Walk through each of the mesh's vertices
		vertices.Reserve(mesh->mNumVertices);
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			CustomVertex vertex;

			vertex.position = SVector3f(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

			if (mesh->HasNormals())
			{
				vertex.Normal = SVector3f::Normalize(SVector3f(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));				
			}
			
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent  = SVector3f::Normalize(SVector3f(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z));
			}

			if (mesh->mTextureCoords[0])
			{
				vertex.texcoord.x = (float)mesh->mTextureCoords[0][j].x;
				vertex.texcoord.y = (float)mesh->mTextureCoords[0][j].y;
			}

			vertices.PushBack(vertex);
		}

		indices.Reserve(mesh->mNumFaces * 3);
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
				indices.PushBack(face.mIndices[k]);
		}

		Mesh& subMesh = meshes.Peek();
		subMesh.SetVertices(vertices);
		subMesh.SetIndices(indices);
		subMesh.SetTextures(textures);
		subMesh.SetMaterialIndex(mesh->mMaterialIndex);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, meshes, LoadedTextures, directory);
	}
}

void ProcessMaterial(const aiScene* scene, SArray<CustomMaterial>& materials, std::string directory)
{
	materials.Reserve(materials.GetSize() + scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* mat = scene->mMaterials[i];

		materials.PushBack(CustomMaterial());
		auto& dstMat = materials.Peek();
		
		dstMat.m_Name = mat->GetName().C_Str();

		aiString path;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string filename = std::string(path.C_Str());
		filename = directory + '\\' + filename;
		filename.replace(filename.end() - 3, filename.end(), "dds");
		dstMat.m_DiffuseMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_NORMALS, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '\\' + filename;
		filename.replace(filename.end() - 3, filename.end(), "dds");
		dstMat.m_NormalMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '\\' + filename;
		filename.replace(filename.end() - 3, filename.end(), "dds");
		dstMat.m_MetallicMap = STextureManager::Load2DTextureFromFile(filename.c_str());
		
		mat->GetTexture(aiTextureType_SHININESS, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '\\' + filename;
		filename.replace(filename.end() - 3, filename.end(), "dds");
		dstMat.m_roughnessMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_AMBIENT, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '\\' + filename;
		filename.replace(filename.end() - 3, filename.end(), "dds");
		dstMat.m_AOMap = STextureManager::Load2DTextureFromFile(filename.c_str());
	}
}

SStaticMesh* SStaticMeshMangaer::LoadStaticMesh(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!pScene)	return nullptr;

	std::string filePath = filename;
	std::string directory = filePath.substr(0, filePath.find_last_of('\\'));

	SArray<CustomMaterial> materials;
	ProcessMaterial(pScene, materials, directory);

	SArray<Mesh> meshes;
	SArray<CustomTexture> LoadedTextures;
	ProcessNode(pScene->mRootNode, pScene, meshes, LoadedTextures, directory);

	// combine vertices and indices
	SArray<CustomVertex> vertices;
	SArray<unsigned int> indices;

	SVertexDescription desc;
	desc.m_iPos         = (eVF_Float3 << 8) | 0;
	desc.m_iNormal      = (eVF_Float3 << 8) | sizeof(SVector3f);
	desc.m_iTexcoord[2] = (eVF_Float3 << 8) | (sizeof(SVector3f) * 2);
	desc.m_iTexcoord[0] = (eVF_Float2 << 8) | (sizeof(SVector3f) * 3);
	desc.stride         = sizeof(CustomVertex);

	for (unsigned int i = 0; i < meshes.GetSize(); i++)
	{
		vertices.Insert(meshes[i].m_vertices, vertices.GetSize());
		indices.Insert(meshes[i].m_indices, indices.GetSize());
	}

	SStaticMesh* staticMesh = new SStaticMesh(filename, g_StaticMeshManager);
	staticMesh->GetMeshBuffer().EnsureVertexBuffer(desc, eBU_Static, vertices.GetSize(), vertices.GetData(), eBF_Vertex);
	staticMesh->GetMeshBuffer().EnsureIndexBuffer(eBU_Static, indices.GetSize(), indices.GetData(), eIF_Int, eBF_Index);

	staticMesh->SetMaterials(materials);

	for (unsigned int i = 0, startIndex = 0, startVertex = 0; i < meshes.GetSize(); i++)
	{
		CustomSubMesh& submesh = staticMesh->AppendSubmesh();
		submesh.iIndexNum    = meshes[i].m_indices.GetSize();
		submesh.iIndexStart  = startIndex;
		submesh.iVertexStart = startVertex;

		startIndex  += submesh.iIndexNum;
		startVertex += meshes[i].m_vertices.GetSize();

		submesh.iMatID = meshes[i].m_matIndex;
	}

	return staticMesh;
}

SStaticMesh* SStaticMeshMangaer::CreateSphere(int radius, int sliceCount, int stackCount)
{
	// ������ϵ����г�stackCountƬ,ÿ�����涼��һ��Բ,Ȼ��ÿ��Բ��ΪsliceCount��
	float theta_step = SMath::PI / stackCount;
	float phi_step   = SMath::TWO_PI / sliceCount;

	SArray<CustomVertex> vertices;
	vertices.Reserve((stackCount - 1) * (sliceCount - 1) + 2);

	CustomVertex topVert;
	topVert.position = SVector3f(0.0f, (float)radius, 0.0f);
	topVert.Normal   = SVector3f(0.0f, 1.0f, 0.0f);
	topVert.Tangent  = SVector3f(1.0f, 0.0f, 0.0f);
	topVert.texcoord = SVector2f(0.0f, 0.0f);

	CustomVertex bottomVert;
	bottomVert.position = SVector3f(0.0f, -(float)(radius), 0.0f);
	bottomVert.Normal   = SVector3f(0.0f, -1.0f, 0.0f);
	bottomVert.Tangent  = SVector3f(-1.0f, 0.0f, 0.0f);
	bottomVert.texcoord = SVector2f(0.0f, 1.0f);
	
	vertices.PushBack(topVert);

	for (int i = 1; i < stackCount; i++)
	{
		float theta = i * theta_step;	// ����ֱ������ļн�,��������Y������ļн�

		for (int j = 0; j <= sliceCount; j++)
		{
			float phi = j * phi_step;

			CustomVertex v;

			v.position.x = radius * SMath::SinRad(theta) * SMath::CosRad(phi);
			v.position.y = radius * SMath::CosRad(theta);
			v.position.z = radius * SMath::SinRad(theta) * SMath::SinRad(phi);

			/*
				����ͨ���ɸ������ޱƽ����ߣ����ҽ�ϼ��ޣ��������Ƶ������ϵ�һ�����������
				�ɲμ���http://netedu.xauat.edu.cn/jpkc/netedu/jpkc/gdsx/homepage/5jxsd/51/513/5308/530806.htm
				��ˣ�Position����theta��ƫ������Ϊ��������
			*/
			v.Tangent.x = -radius * SMath::SinRad(theta) * SMath::SinRad(phi);
			v.Tangent.y = 0.0f;
			v.Tangent.z = radius * SMath::SinRad(theta) * SMath::CosRad(phi);
			v.Tangent.Normalize();

			// ���߼�ΪԲ��ָ��õ����������˸պõ��ڵ��λ�������ֵ
			v.Normal = SVector3f::Normalize(v.position);

			v.texcoord.x = phi / SMath::TWO_PI;
			v.texcoord.y = theta / SMath::PI;

			vertices.PushBack(v);
		}
	}

	vertices.PushBack(bottomVert);

	SArray<int> indices;

	// ����
	for (int i = 1; i <= sliceCount; i++)
	{
		indices.PushBack(0);
		indices.PushBack(i + 1);
		indices.PushBack(i);
	}

	// �����ڲ��Ļ�
	int baseIndex = 1, ringVertexCount = sliceCount + 1;

	for (int i = 0; i < stackCount - 2; i++)
	{
		for (int j = 0; j < sliceCount; j++)
		{
			indices.PushBack(baseIndex + i * ringVertexCount + j);
			indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
			indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);
			
			indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);
			indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
			indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	// ����ײ��Ļ�
	int southPolarIndex = vertices.GetSize() - 1;
	baseIndex = southPolarIndex - ringVertexCount;

	for(int i = 0; i < sliceCount; i++)
	{
		indices.PushBack(southPolarIndex);
		indices.PushBack(baseIndex + i);
		indices.PushBack(baseIndex + i + 1);
	}

	SArray<CustomMaterial> materials;
	CustomMaterial mat;
	mat.m_DiffuseMap   = STextureManager::Load2DTextureFromFile("../asset/textures/albedo.dds");
	//mat.m_NormalMap    = STextureManager::Load2DTextureFromFile("../asset/textures/normal.dds");
	mat.m_NormalMap = STextureManager::Load2DTexturePNG("../asset/textures/normal.png");
	mat.m_MetallicMap  = STextureManager::Load2DTextureFromFile("../asset/textures/metallic.dds");
	mat.m_roughnessMap = STextureManager::Load2DTextureFromFile("../asset/textures/roughness.dds");
	mat.m_AOMap        = STextureManager::Load2DTextureFromFile("../asset/textures/ao.dds");
	mat.m_Name         = "diffuse";
	materials.PushBack(mat);

	SVertexDescription desc;
	desc.m_iPos         = (eVF_Float3 << 8) | 0;
	desc.m_iNormal      = (eVF_Float3 << 8) | sizeof(SVector3f);
	desc.m_iTexcoord[2] = (eVF_Float3 << 8) | (sizeof(SVector3f) * 2);
	desc.m_iTexcoord[0] = (eVF_Float2 << 8) | (sizeof(SVector3f) * 3);
	desc.stride         = sizeof(CustomVertex);

	SStaticMesh* staticMesh = new SStaticMesh("", g_StaticMeshManager);
	staticMesh->GetMeshBuffer().EnsureVertexBuffer(desc, eBU_Static, vertices.GetSize(), vertices.GetData(), eBF_Vertex);
	staticMesh->GetMeshBuffer().EnsureIndexBuffer(eBU_Static, indices.GetSize(), indices.GetData(), eIF_Int, eBF_Index);

	staticMesh->SetMaterials(materials);

	CustomSubMesh& submesh = staticMesh->AppendSubmesh();
	submesh.iIndexNum    = indices.GetSize();
	submesh.iIndexStart  = 0;
	submesh.iVertexStart = 0;
	submesh.iMatID       = 0;

	return staticMesh;
}