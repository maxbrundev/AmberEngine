#pragma once

#include "AmberEngine/API/Export.h"

#include <assimp/scene.h>

#include "AmberEngine/Geometry/Vertex.h"

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources::Parsers
{
	class API_AMBERENGINE AssimpParser
	{
	public:
		bool LoadModel(const std::string& p_filePath, Model& p_model);

	private:
		void ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes, std::array<Material*, 255>&);
		void ProcessMesh(const aiMatrix4x4* p_transform, const aiMesh* p_mesh, const aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
		void ProcessMaterial(const aiMesh* p_mesh, const aiScene* p_scene, Material& p_outMaterial);

		void LoadTexturesFromMaterial(const aiMaterial* p_mat, aiTextureType p_type, ETextureType p_textureType, std::unordered_map<ETextureType, std::shared_ptr<Texture>>& p_outTextures);

		std::vector<std::shared_ptr<Texture>> m_loadedTextures;
		std::string m_directory;
	};
}
