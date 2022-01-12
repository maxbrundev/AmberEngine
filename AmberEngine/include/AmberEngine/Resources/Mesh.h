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
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, std::vector<std::shared_ptr<Texture>> p_textures);
		~Mesh();

		void Bind() const;
		void Unbind() const;
		void BindMaterialTextures() const;

		std::function<void(std::string, int)> SetTextureUniformCallback;

		uint32_t GetVertexCount() const;
		uint32_t GetIndexCount() const;

	private:
		void InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

	private:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		std::vector<std::shared_ptr<Texture>> m_textures;

		Buffers::VertexArray m_vertexArray;
		std::unique_ptr<Buffers::VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Buffers::IndexBuffer> m_indexBuffer;
	};
}