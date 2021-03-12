#include "Amberpch.h"

#include <stb_image.h>

#include "AmberEngine/Resources/AssimpModel.h"

unsigned int AmberEngine::Resources::AssimpModel::LoadTexture(const std::string& p_filePath, const std::string& p_directory)
{
	unsigned int textureId;

	int width;
	int height;
	int bitsPerPixel;

	std::string filename = p_filePath;

	filename = p_directory + '/' + filename;

	glGenTextures(1, &textureId);

	unsigned char* dataBuffer = stbi_load(filename.c_str(), &width, &height, &bitsPerPixel, 0);

	if (dataBuffer)
	{
		GLenum format;
		if (bitsPerPixel == 1)
			format = GL_RED;
		else if (bitsPerPixel == 3)
			format = GL_RGB;
		else if (bitsPerPixel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(dataBuffer);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << p_filePath << std::endl;
		stbi_image_free(dataBuffer);
	}

	return textureId;
}

AmberEngine::Resources::AssimpModel::AssimpModel(const std::string& p_filePath)
{
	LoadModel(p_filePath);
}

void AmberEngine::Resources::AssimpModel::Draw(Shader& p_shader)
{
	for (int i = 0; i < m_meshes.size(); i++)
		m_meshes[i].BindBuffers(p_shader);
}

AmberEngine::Resources::AssimpModel::~AssimpModel()
{
	for (int i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i].DeleteBuffers();
	}
}

void AmberEngine::Resources::AssimpModel::LoadModel(const std::string& p_filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = p_filePath.substr(0, p_filePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void AmberEngine::Resources::AssimpModel::ProcessNode(aiNode* p_node, const aiScene* p_scene)
{
	for (int i = 0; i < p_node->mNumMeshes; i++)
	{
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, p_scene));
	}

	for (int i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(p_node->mChildren[i], p_scene);
	}
}

AmberEngine::Resources::AssimpMesh AmberEngine::Resources::AssimpModel::ProcessMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
	std::vector<AssimpVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AssimpTextureData> textures;

	for (int i = 0; i < p_mesh->mNumVertices; i++)
	{
		AssimpVertex vertex;
		glm::vec3 vector;

		vector.x = p_mesh->mVertices[i].x;
		vector.y = p_mesh->mVertices[i].y;
		vector.z = p_mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = p_mesh->mNormals[i].x;
		vector.y = p_mesh->mNormals[i].y;
		vector.z = p_mesh->mNormals[i].z;
		vertex.normal = vector;

		if (p_mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
		
			vec.x = p_mesh->mTextureCoords[0][i].x;
			vec.y = p_mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		
		vector.x = p_mesh->mTangents[i].x;
		vector.y = p_mesh->mTangents[i].y;
		vector.z = p_mesh->mTangents[i].z;
		vertex.tangent = vector;
		
		vector.x = p_mesh->mBitangents[i].x;
		vector.y = p_mesh->mBitangents[i].y;
		vector.z = p_mesh->mBitangents[i].z;
		vertex.bitangent = vector;
		vertices.push_back(vertex);
	}
	
	for (int i = 0; i < p_mesh->mNumFaces; i++)
	{
		aiFace face = p_mesh->mFaces[i];
		
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

	std::vector<AssimpTextureData> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
	std::vector<AssimpTextureData> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
	std::vector<AssimpTextureData> normalMaps = LoadMaterial(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
	std::vector<AssimpTextureData> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return AssimpMesh(vertices, indices, textures);
}

std::vector<AmberEngine::Resources::AssimpTextureData> AmberEngine::Resources::AssimpModel::LoadMaterial(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<AssimpTextureData> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		bool skip = false;
		for (int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			AssimpTextureData texture;
			texture.id = LoadTexture(str.C_Str(), m_directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}
	return textures;
}