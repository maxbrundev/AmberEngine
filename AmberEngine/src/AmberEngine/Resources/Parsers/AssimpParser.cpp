#include "Amberpch.h"

#include "AmberEngine/Resources/Parsers/AssimpParser.h"

#include "AmberEngine/Resources/Loaders/TextureLoader.h"
#include "AmberEngine/Tools/Utils/String.h"

bool AmberEngine::Resources::AssimpParser::LoadModel(const std::string& p_filePath, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials)
{
	m_directory = Utils::String::ExtractDirectoryFromPath(p_filePath);
	
	Assimp::Importer import;
	
	const aiScene* scene = import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	ProcessMaterials(scene, p_materials);

	aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_meshes);

	m_loadedTextures.clear();

	return true;
}

void AmberEngine::Resources::AssimpParser::ProcessMaterials(const aiScene * p_scene, std::vector<std::string>& p_materials)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; ++i)
	{
		aiMaterial* material = p_scene->mMaterials[i];
		
		if (material)
		{
			aiString name;
			
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			p_materials.emplace_back(name.C_Str());
		}
	}
}

void AmberEngine::Resources::AssimpParser::ProcessNode(aiMatrix4x4* p_transform, aiNode* p_node, const aiScene * p_scene, std::vector<Mesh*>& p_meshes)
{
	aiMatrix4x4 nodeTransformation = *p_transform * p_node->mTransformation;

	// Process all the node's meshes (if any)
	for (uint32_t i = 0; i < p_node->mNumMeshes; ++i)
	{
		std::vector<Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Texture*> textures;
		
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		
		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices, textures);
		p_meshes.push_back(new Mesh(vertices, indices, textures)); // The model will handle mesh destruction
	}

	// Then do the same for each of its children
	for (uint32_t i = 0; i < p_node->mNumChildren; ++i)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}

void AmberEngine::Resources::AssimpParser::ProcessMesh(aiMatrix4x4* p_transform, aiMesh* p_mesh, const aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices, std::vector<Texture*>& p_outTextures)
{
	const aiMatrix4x4 meshTransformation = *p_transform;

	for (uint32_t i = 0; i < p_mesh->mNumVertices; ++i)
	{
		const aiVector3D position  = meshTransformation * p_mesh->mVertices[i];
		const aiVector3D normal    = meshTransformation * (p_mesh->mNormals ? p_mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		const aiVector3D texCoords = p_mesh->mTextureCoords[0] ? p_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		const aiVector3D tangent   = p_mesh->mTangents ? meshTransformation * p_mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		const aiVector3D bitangent = p_mesh->mBitangents ? meshTransformation * p_mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

		p_outVertices.push_back
		(
			{
				position.x,
				position.y,
				position.z,
				texCoords.x,
				texCoords.y,
				normal.x,
				normal.y,
				normal.z,
				tangent.x,
				tangent.y,
				tangent.z,
				bitangent.x,
				bitangent.y,
				bitangent.z
			}
		);
	}

	for (uint32_t faceID = 0; faceID < p_mesh->mNumFaces; ++faceID)
	{
		auto& face = p_mesh->mFaces[faceID];

		for (size_t indexID = 0; indexID < 3; ++indexID)
		{
			p_outIndices.push_back(face.mIndices[indexID]);
		}
	}

	aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

	std::vector<Texture*> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, Settings::ETextureType::DIFFUSE);
	p_outTextures.insert(p_outTextures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture*> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, Settings::ETextureType::SPECULAR);
	p_outTextures.insert(p_outTextures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture*> normalMaps = LoadMaterial(material, aiTextureType_NORMALS, Settings::ETextureType::NORMAL_MAP);
	p_outTextures.insert(p_outTextures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture*> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT, Settings::ETextureType::HEIGHT_MAP);
	p_outTextures.insert(p_outTextures.end(), heightMaps.begin(), heightMaps.end());
}

std::vector<AmberEngine::Resources::Texture*> AmberEngine::Resources::AssimpParser::LoadMaterial(aiMaterial* p_mat,
	aiTextureType p_type, Settings::ETextureType p_textureType)
{
	std::vector<Texture*> textures;
	for (int i = 0; i < p_mat->GetTextureCount(p_type); i++)
	{
		aiString str;
		p_mat->GetTexture(p_type, i, &str);

		bool skip = false;
		for (int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j]->name.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}
		
		if (!skip)
		{
			std::string path = m_directory + str.C_Str();

			Texture* texture = AmberEngine::Resources::TextureLoader::Create(path, Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, Settings::ETextureFilteringMode::NEAREST, p_textureType, false, true);

			textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}
	return textures;
}
