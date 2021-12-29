#include "Amberpch.h"

#include "AmberEngine/Resources/Parsers/AssimpParser.h"

#include <chrono>

#include "AmberEngine/Resources/Loaders/TextureLoader.h"
#include "AmberEngine/Tools/Utils/String.h"

bool AmberEngine::Resources::AssimpParser::LoadModel(const std::string& p_filePath, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials)
{
	m_directory = Utils::String::ExtractDirectoryFromPath(p_filePath);

	//const auto start = std::chrono::high_resolution_clock::now();

	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	p_materials.reserve(scene->mNumMaterials);

	ProcessMaterials(scene, p_materials);

	const aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_meshes);

	m_loadedTextures.clear();

	//const auto end = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed = end - start;
	//std::cout << "duration: " << elapsed.count() << " s\n";

	return true;
}

void AmberEngine::Resources::AssimpParser::ProcessMaterials(const aiScene* p_scene, std::vector<std::string>& p_materials)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; ++i)
	{
		if (const aiMaterial* material = p_scene->mMaterials[i])
		{
			aiString name;

			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			p_materials.emplace_back(name.C_Str());
		}
	}
}

void AmberEngine::Resources::AssimpParser::ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene * p_scene, std::vector<Mesh*>& p_meshes)
{
	const aiMatrix4x4 nodeTransformation = *p_transform * p_node->mTransformation;

	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	// Process all the node's meshes (if any)
	for (uint32_t i = 0; i < p_node->mNumMeshes; i++)
	{
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		const aiMaterial* material = p_scene->mMaterials[mesh->mMaterialIndex];

		const uint64_t textureCount = material->GetTextureCount(aiTextureType_DIFFUSE)
			+ material->GetTextureCount(aiTextureType_SPECULAR)
			+ material->GetTextureCount(aiTextureType_NORMALS)
			+ material->GetTextureCount(aiTextureType_HEIGHT);


		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces);
		textures.reserve(textureCount);

		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices, textures);

		p_meshes.push_back(new Mesh(vertices, indices, std::move(textures))); // The model will handle mesh destruction

		vertices.clear();
		indices.clear();
		textures.clear();
	}

	// Then do the same for each of its children
	for (uint32_t i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}

void AmberEngine::Resources::AssimpParser::ProcessMesh(const aiMatrix4x4* p_transform, const aiMesh* p_mesh, const aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices, std::vector<std::shared_ptr<Texture>>& p_outTextures)
{
	const aiMatrix4x4 meshTransformation = *p_transform;

	for (uint32_t i = 0; i < p_mesh->mNumVertices; i++)
	{
		const aiVector3D position = meshTransformation * p_mesh->mVertices[i];
		const aiVector3D normal = meshTransformation * (p_mesh->mNormals ? p_mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		const aiVector3D texCoords = p_mesh->mTextureCoords[0] ? p_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		const aiVector3D tangent = p_mesh->mTangents ? meshTransformation * p_mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
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

	for (uint32_t faceID = 0; faceID < p_mesh->mNumFaces; faceID++)
	{
		const auto& face = p_mesh->mFaces[faceID];
		p_outIndices.insert(p_outIndices.end(), { face.mIndices[0], face.mIndices[1], face.mIndices[2] });
	}

	const aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

	std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, Settings::ETextureType::DIFFUSE);
	p_outTextures.insert(p_outTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
	std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, Settings::ETextureType::SPECULAR);
	p_outTextures.insert(p_outTextures.end(), specularMaps.begin(), specularMaps.end());
	
	std::vector<std::shared_ptr<Texture>> normalMaps = LoadMaterial(material, aiTextureType_NORMALS, Settings::ETextureType::NORMAL_MAP);
	p_outTextures.insert(p_outTextures.end(), normalMaps.begin(), normalMaps.end());
	
	std::vector<std::shared_ptr<Texture>> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT, Settings::ETextureType::HEIGHT_MAP);
	p_outTextures.insert(p_outTextures.end(), heightMaps.begin(), heightMaps.end());
}

std::vector<std::shared_ptr<AmberEngine::Resources::Texture>> AmberEngine::Resources::AssimpParser::LoadMaterial(const aiMaterial* p_mat, aiTextureType p_type, Settings::ETextureType p_textureType)
{
	std::vector<std::shared_ptr<Texture>> textures;

	for (int i = 0; i < p_mat->GetTextureCount(p_type); i++)
	{
		aiString str;
		p_mat->GetTexture(p_type, i, &str);

		bool isTextureAlreadyLoaded = false;

		for (const auto& texture : m_loadedTextures)
		{
			if (std::strcmp(texture->name.data(), str.C_Str()) == 0)
			{
				textures.push_back(texture);

				isTextureAlreadyLoaded = true;

				break;
			}
		}

		if (!isTextureAlreadyLoaded)
		{
			const std::string path = m_directory + str.C_Str();

			std::shared_ptr<Texture> texture(TextureLoader::Create(path, Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, Settings::ETextureFilteringMode::NEAREST, p_textureType, false, true));

			textures.push_back(texture);

			m_loadedTextures.push_back(texture);
		}
	}
	return textures;
}