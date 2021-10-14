#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Geometry/Vertex.h"

#include "AmberEngine/Buffers/VertexArray.h"
#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Buffers/IndexBuffer.h"

#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Mesh
	{
	public:
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, const std::vector<Texture*>& p_textures);
		~Mesh();

		void Bind();
		void Unbind();
		void BindMaterialTextures();

		std::function<void(std::string, int)> SetTextureUniformCallback;

	private:
		void InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

	public:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		std::vector<Texture*> m_textures;

		Buffers::VertexArray m_vertexArray;
		std::unique_ptr<Buffers::VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Buffers::IndexBuffer> m_indexBuffer;
	};
}