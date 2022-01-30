#include "Amberpch.h"

#include "AmberEngine/Resources/Parsers/AssimpParser.h"

//#include <chrono>

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Tools/Utils/String.h"

bool AmberEngine::Resources::Parsers::AssimpParser::LoadModel(const std::string& p_filePath, Model& p_model)
{
	m_directory = Utils::String::ExtractDirectoryFromPath(p_filePath);

	//const auto start = std::chrono::high_resolution_clock::now();

	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	//ProcessMaterialsNames(scene, p_model.GetMaterialNames());

	const aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_model.GetMeshes());

	m_loadedTextures.clear();

	p_model.BindTextureCallback();

	//const auto end = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed = end - start;
	//std::cout << "duration: " << elapsed.count() << " s\n";

	return true;
}

void AmberEngine::Resources::Parsers::AssimpParser::ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes)
{
	const aiMatrix4x4 nodeTransformation = *p_transform * p_node->mTransformation;

	for (uint32_t i = 0; i < p_node->mNumMeshes; i++)
	{
		const aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];

		std::vector<Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices);
		ProcessMaterial(mesh, p_scene, textures);

		p_meshes.push_back(new Mesh(vertices, indices, std::move(textures)));
	}

	for (uint32_t i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}

void AmberEngine::Resources::Parsers::AssimpParser::ProcessMesh(const aiMatrix4x4* p_transform, const aiMesh* p_mesh, const aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
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
}

void AmberEngine::Resources::Parsers::AssimpParser::ProcessMaterialsNames(const aiScene* p_scene, std::vector<std::string>& p_outMaterialsNames)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; ++i)
	{
		if (const aiMaterial* material = p_scene->mMaterials[i])
		{
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);

			p_outMaterialsNames.emplace_back(name.C_Str());
		}
	}
}

void AmberEngine::Resources::Parsers::AssimpParser::ProcessMaterial(const aiMesh* p_mesh, const aiScene* p_scene, std::vector<std::shared_ptr<Texture>>& p_outTextures)
{
	const aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];
	
	std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadTexturesFromMaterial(material, aiTextureType_DIFFUSE, Settings::ETextureType::DIFFUSE_MAP);
	p_outTextures.insert(p_outTextures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
	
	std::vector<std::shared_ptr<Texture>> specularMaps = LoadTexturesFromMaterial(material, aiTextureType_SPECULAR, Settings::ETextureType::SPECULAR_MAP);
	p_outTextures.insert(p_outTextures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
	
	std::vector<std::shared_ptr<Texture>> normalMaps = LoadTexturesFromMaterial(material, aiTextureType_NORMALS, Settings::ETextureType::NORMAL_MAP);
	p_outTextures.insert(p_outTextures.end(), std::make_move_iterator(normalMaps.begin()), std::make_move_iterator(normalMaps.end()));
	
	std::vector<std::shared_ptr<Texture>> heightMaps = LoadTexturesFromMaterial(material, aiTextureType_HEIGHT, Settings::ETextureType::HEIGHT_MAP);
	p_outTextures.insert(p_outTextures.end(), std::make_move_iterator(heightMaps.begin()), std::make_move_iterator(heightMaps.end()));

	std::vector<std::shared_ptr<Texture>> maskMaps = LoadTexturesFromMaterial(material, aiTextureType_OPACITY, Settings::ETextureType::MASK_MAP);
	p_outTextures.insert(p_outTextures.end(), std::make_move_iterator(maskMaps.begin()), std::make_move_iterator(maskMaps.end()));
}

std::vector<std::shared_ptr<AmberEngine::Resources::Texture>> AmberEngine::Resources::Parsers::AssimpParser::LoadTexturesFromMaterial(const aiMaterial* p_mat, aiTextureType p_type, Settings::ETextureType p_textureType)
{
	std::vector<std::shared_ptr<Texture>> textures;

	for (uint32_t i = 0; i < p_mat->GetTextureCount(p_type); i++)
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
			std::string path = m_directory + str.C_Str();

			std::shared_ptr<Texture> texture(Loaders::TextureLoader::Create(std::move(path), Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, Settings::ETextureFilteringMode::NEAREST, p_textureType, false, true));

			if(texture == nullptr)
			{
				continue;
			}

			textures.push_back(texture);

			m_loadedTextures.push_back(texture);
		}
	}
	return textures;
}
