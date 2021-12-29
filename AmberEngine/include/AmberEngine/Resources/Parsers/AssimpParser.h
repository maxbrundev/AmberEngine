#pragma once

#include "AmberEngine/API/Export.h"

#include <assimp/scene.h>

#include "AmberEngine/Resources/Mesh.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE AssimpParser
	{
	public:
		bool LoadModel(const std::string& p_filePath, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials);

	private:
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
		void ProcessNode(const aiMatrix4x4* p_transform, const struct aiNode* p_node, const struct aiScene* p_scene, std::vector<Mesh*>& p_meshes);
		void ProcessMesh(const aiMatrix4x4* p_transform, const struct aiMesh* p_mesh, const struct aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices, std::vector<std::shared_ptr<Texture>>& p_outTextures);
		std::vector<std::shared_ptr<Texture>> LoadMaterial(const aiMaterial* p_mat, aiTextureType p_type, Settings::ETextureType p_textureType);

		std::vector<std::shared_ptr<Texture>> m_loadedTextures;
		std::string m_directory;
	};
}