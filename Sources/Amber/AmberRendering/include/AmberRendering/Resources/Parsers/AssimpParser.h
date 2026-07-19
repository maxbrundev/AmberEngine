#pragma once


#include <assimp/scene.h>

#include "EModelParserFlags.h"
#include "AmberRendering/Geometry/Vertex.h"

#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/Texture.h"

namespace AmberRendering::Resources::Parsers
{
	class AssimpParser
	{
	public:
		bool LoadModel(const std::string& p_filePath, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials, EModelParserFlags p_parserFlags);
		std::map<int, std::vector<std::tuple<ETextureType, std::string>>> textureData;

	private:
		void ProcessNode(const aiMatrix4x4* p_transform, const aiNode* p_node, const aiScene* p_scene, std::vector<Mesh*>& p_meshes);
		void ProcessMesh(const aiMatrix4x4* p_transform, const aiMesh* p_mesh, const aiScene* p_scene, std::vector<AmberRendering::Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);
	};
}
