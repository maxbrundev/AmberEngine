#include "Amberpch.h"

#include "AmberEngine/Resources/Parsers/AssimpParser.h"

#include "AmberEngine/Resources/Mesh.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

#include "AmberEngine/Tools/Utils/String.h"

bool AmberEngine::Resources::Parsers::AssimpParser::LoadModel(const std::string& p_filePath, Model& p_model)
{
	m_directory = Utils::String::ExtractDirectoryFromPath(p_filePath);

	//const auto start = std::chrono::high_resolution_clock::now();

	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_GlobalScale);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	const aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_model.GetMeshes(), p_model.GetMaterials());

	m_loadedTextures.clear();

	//const auto end = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> elapsed = end - start;
	//std::cout << "duration: " << elapsed.count() << " s\n";

	return true;
}

void AmberEngine::Resources::Parsers::AssimpParser::ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes, std::array<Material*, 255>& p_materials)
{
	const aiMatrix4x4 nodeTransformation = *p_transform * p_node->mTransformation;

	for (uint32_t i = 0; i < p_node->mNumMeshes; i++)
	{
		const aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];

		std::vector<Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;

		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices);

		p_meshes.push_back(new Mesh(vertices, indices, mesh->mMaterialIndex));

		if(p_materials[mesh->mMaterialIndex] == nullptr)
		{
			p_materials[mesh->mMaterialIndex] = new Material();
			ProcessMaterial(mesh, p_scene, *p_materials[mesh->mMaterialIndex]);
		}
	}

	for (uint32_t i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes, p_materials);
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

void AmberEngine::Resources::Parsers::AssimpParser::ProcessMaterial(const aiMesh* p_mesh, const aiScene* p_scene, Material& p_outMaterial)
{
	const aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

	aiString name;
	aiGetMaterialString(material, AI_MATKEY_NAME, &name);

	p_outMaterial.SetName(std::move(std::string(name.C_Str())));

	std::unordered_map<ETextureType, std::shared_ptr<Texture>> textures;

	LoadTexturesFromMaterial(material, aiTextureType_DIFFUSE, ETextureType::DIFFUSE_MAP, textures);
	LoadTexturesFromMaterial(material, aiTextureType_SPECULAR, ETextureType::SPECULAR_MAP, textures);
	LoadTexturesFromMaterial(material, aiTextureType_NORMALS, ETextureType::NORMAL_MAP, textures);
	LoadTexturesFromMaterial(material, aiTextureType_HEIGHT, ETextureType::HEIGHT_MAP, textures);
	LoadTexturesFromMaterial(material, aiTextureType_OPACITY, ETextureType::MASK_MAP, textures);

	p_outMaterial.FillTextures(std::move(textures));
}

void AmberEngine::Resources::Parsers::AssimpParser::LoadTexturesFromMaterial(const aiMaterial* p_mat, aiTextureType p_type, ETextureType p_textureType, std::unordered_map<ETextureType, std::shared_ptr<Texture>>& p_outTextures)
{
	for (uint32_t i = 0; i < p_mat->GetTextureCount(p_type); i++)
	{
		aiString str;
		p_mat->GetTexture(p_type, i, &str);

		const auto& textureLoaded = std::find_if(m_loadedTextures.begin(), m_loadedTextures.end(), [&](const std::shared_ptr<Texture>& p_texture)
		{
			return p_texture->name == str.C_Str();
		});

		if (textureLoaded != m_loadedTextures.end())
		{
			p_outTextures[p_textureType] = *textureLoaded;
			continue;
		}

		std::shared_ptr<Texture> texture(Loaders::TextureLoader::Create(m_directory + str.C_Str(), ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, ETextureFilteringMode::NEAREST, p_textureType, false, true));

		if (texture == nullptr)
			continue;

		p_outTextures[texture->type] = texture;

		m_loadedTextures.push_back(texture);
	}
}
