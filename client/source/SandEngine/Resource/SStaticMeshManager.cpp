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
	SVector3f Binormal;
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
				vertex.Normal = SVector3f(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
			}
			
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent  = SVector3f(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				vertex.Binormal = SVector3f(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z);
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
		filename = directory + '/' + filename;
		dstMat.m_DiffuseMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_NORMALS, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '/' + filename;
		dstMat.m_NormalMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '/' + filename;
		dstMat.m_MetallicMap = STextureManager::Load2DTextureFromFile(filename.c_str());

		mat->GetTexture(aiTextureType_SHININESS, 0, &path);
		filename = std::string(path.C_Str());
		filename = directory + '/' + filename;
		dstMat.m_roughnessMap = STextureManager::Load2DTextureFromFile(filename.c_str());
	}
}

SStaticMesh* SStaticMeshMangaer::LoadStaticMesh(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!pScene)	return nullptr;

	std::string filePath = filename;
	std::string directory = filePath.substr(0, filePath.find_last_of('/'));;

	SArray<CustomMaterial> materials;
	ProcessMaterial(pScene, materials, directory);

	SArray<Mesh> meshes;
	SArray<CustomTexture> LoadedTextures;
	ProcessNode(pScene->mRootNode, pScene, meshes, LoadedTextures, directory);

	// combine vertices and indices
	SArray<CustomVertex> vertices;
	SArray<unsigned int> indices;

	SVertexDescription desc;
	desc.m_iPos         = (eVF_Float3 << 24) | 0;
	desc.m_iNormal      = (eVF_Float3 << 24) | sizeof(SVector3f);
	desc.m_iTexcoord[2] = (eVF_Float3 << 24) | (sizeof(SVector3f) * 2);
	desc.m_iTexcoord[3] = (eVF_Float3 << 24) | (sizeof(SVector3f) * 3);
	desc.m_iTexcoord[0] = (eVF_Float2 << 24) | (sizeof(SVector3f) * 4);

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
		submesh.iVertexNum   = meshes[i].m_vertices.GetSize();
		submesh.iIndexStart  = startIndex;
		submesh.iVertexStart = startVertex;

		startIndex  += submesh.iIndexNum;
		startVertex += submesh.iVertexNum;

		submesh.iMatID = meshes[i].m_matIndex;
	}

	return staticMesh;
}