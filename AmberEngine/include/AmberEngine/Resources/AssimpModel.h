#pragma once

#include <assimp/scene.h>

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/AssimpMesh.h"

//TODO: Refactor.

namespace AmberEngine::Resources
{
	class API_AMBERENGINE AssimpModel
	{
	public:
		AssimpModel(const std::string& p_filePath);
		~AssimpModel();
		
		void Draw(Shader& p_shader);

	public:
		std::vector<Texture*> m_loadedTextures;
		std::vector<AssimpMesh> m_meshes;
		std::string m_directory;
		
	private:
		void LoadModel(const std::string& p_filePath);
		void ProcessNode(aiNode* p_node, const aiScene* p_scene);
		AssimpMesh ProcessMesh(aiMesh* p_mesh, const aiScene* p_scene);
		std::vector<Texture*> LoadMaterial(aiMaterial* p_mat, aiTextureType p_type, const std::string& p_typeName);
	};
}