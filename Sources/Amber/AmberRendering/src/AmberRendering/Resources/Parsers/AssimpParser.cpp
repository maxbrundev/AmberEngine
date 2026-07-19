#include "Amberpch.h"

#include "AmberRendering/Resources/Parsers/AssimpParser.h"

#include "AmberRendering/Resources/Mesh.h"
#include "AmberRendering/Resources/Loaders/TextureLoader.h"


bool AmberRendering::Resources::Parsers::AssimpParser::LoadModel(const std::string& p_filePath, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials, EModelParserFlags p_parserFlags)
{
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(p_filePath, static_cast<unsigned int>(p_parserFlags));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	const aiMatrix4x4 identity;

	ProcessNode(&identity, scene->mRootNode, scene, p_meshes);
	ProcessMaterials(scene, p_materials);
	return true;
}

void AmberRendering::Resources::Parsers::AssimpParser::ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes)
{
	const aiMatrix4x4 nodeTransformation = *p_transform * p_node->mTransformation;

	for (uint32_t i = 0; i < p_node->mNumMeshes; i++)
	{
		const aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];

		std::vector<AmberRendering::Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;

		ProcessMesh(&nodeTransformation, mesh, p_scene, vertices, indices);

		p_meshes.push_back(new Mesh(vertices, indices, mesh->mMaterialIndex));
	}

	for (uint32_t i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(&nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}

void AmberRendering::Resources::Parsers::AssimpParser::ProcessMesh(const aiMatrix4x4* p_transform, const aiMesh* p_mesh, const aiScene* p_scene, std::vector<AmberRendering::Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
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

void AmberRendering::Resources::Parsers::AssimpParser::ProcessMaterials(const aiScene* p_scene, std::vector<std::string>& p_materials)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; i++)
	{
		aiMaterial* material = p_scene->mMaterials[i];
		if (material)
		{
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			p_materials.push_back(name.C_Str());

			aiString str;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == aiReturn_SUCCESS && str.C_Str()[0] != '\0')
			{
				textureData[i].emplace_back(ETextureType::DIFFUSE_MAP, str.C_Str());
			}

			if (material->GetTexture(aiTextureType_SPECULAR, 0, &str) == aiReturn_SUCCESS && str.C_Str()[0] != '\0')
			{
				textureData[i].emplace_back(ETextureType::SPECULAR_MAP, str.C_Str());
			}
		}
	}
}
