#pragma once

#include "AmberEngine/API/Export.h"

#include <assimp/scene.h>

#include "AmberEngine/Resources/AssimpMesh.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE AssimpParser
	{
	public:
		bool LoadModel(const std::string& p_filePath, std::vector<AssimpMesh*>& p_meshes, std::vector<std::string>& p_materials);
		
	private:
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
		void ProcessNode(aiMatrix4x4* p_transform, struct aiNode* p_node, const struct aiScene* p_scene, std::vector<AssimpMesh*>& p_meshes);
		void ProcessMesh(aiMatrix4x4* p_transform, struct aiMesh* p_mesh, const struct aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices, std::vector<Texture*>& p_outTextures);
		std::vector<Texture*> LoadMaterial(aiMaterial* p_mat, aiTextureType p_type, const std::string& p_typeName);

		std::vector<Texture*> m_loadedTextures;
		std::string m_directory;
	};
}
