#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include "AmberEngine/Resources/AssimpMesh.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE AssimpModel
	{
	public:
		std::vector<TextureData> m_loadedTextures;
		std::vector<AssimpMesh> m_meshes;
		std::string m_directory;
	public:
		AssimpModel(const std::string& p_filePath);
		void Draw();
		~AssimpModel();

	private:
		void LoadModel(const std::string& p_filePath);
		void ProcessNode(aiNode* p_node, const aiScene* p_scene);
		AssimpMesh ProcessMesh(aiMesh* p_mesh, const aiScene* p_scene);
		std::vector<TextureData> LoadMaterial(aiMaterial* p_mat, aiTextureType p_type, const std::string& p_typeName);
		unsigned int LoadTexture(const std::string& p_filePath, const std::string& p_directory);
	};
}