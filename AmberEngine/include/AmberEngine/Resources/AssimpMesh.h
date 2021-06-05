#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Geometry/Vertex.h"
#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE AssimpMesh
	{
	public:
		AssimpMesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, std::vector<Texture*> p_textures);
		~AssimpMesh();

		void Bind();
		void Unbind();

		void BindMaterialTextures(Shader& p_shader);
		void DeleteBuffers();

	private:
		void InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

	public:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		std::vector<Texture*> m_textures;

		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;
	};
}