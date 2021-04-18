#include "Amberpch.h"

#include "AmberEngine/Resources/AssimpModel.h"

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
	std::vector<Texture*> textures;

	for (int i = 0; i < p_mesh->mNumVertices; i++)
	{
		AssimpVertex vertex;
		glm::vec3 vector;

		vector.x = p_mesh->mVertices[i].x;
		vector.y = p_mesh->mVertices[i].y;
		vector.z = p_mesh->mVertices[i].z;
		vertex.position = vector;

		if(p_mesh->HasNormals())
		{
			vector.x = p_mesh->mNormals[i].x;
			vector.y = p_mesh->mNormals[i].y;
			vector.z = p_mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (p_mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
		
			vec.x = p_mesh->mTextureCoords[0][i].x;
			vec.y = p_mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		if(p_mesh->HasTangentsAndBitangents())
		{
			vector.x = p_mesh->mTangents[i].x;
			vector.y = p_mesh->mTangents[i].y;
			vector.z = p_mesh->mTangents[i].z;
			vertex.tangent = vector;

			vector.x = p_mesh->mBitangents[i].x;
			vector.y = p_mesh->mBitangents[i].y;
			vector.z = p_mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		
		vertices.push_back(vertex);
	}
	
	for (int i = 0; i < p_mesh->mNumFaces; i++)
	{
		aiFace face = p_mesh->mFaces[i];
		
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

	std::vector<Texture*> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
	std::vector<Texture*> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	
	std::vector<Texture*> normalMaps = LoadMaterial(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	
	std::vector<Texture*> heightMaps = LoadMaterial(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return AssimpMesh(vertices, indices, textures);
}

std::vector<AmberEngine::Resources::Texture*> AmberEngine::Resources::AssimpModel::LoadMaterial(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture*> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		bool skip = false;
		for (int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j]->m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::string filename = str.C_Str();

			filename = m_directory + '/' + filename;
			
			Texture* currentTexture = new Texture(filename);
			currentTexture->m_type = typeName;
			
			textures.emplace_back(currentTexture);
			m_loadedTextures.push_back(currentTexture);
		}
	}
	return textures;
}