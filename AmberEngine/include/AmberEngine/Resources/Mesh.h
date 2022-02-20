#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Geometry/Vertex.h"

#include "AmberEngine/Buffers/VertexArray.h"
#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Buffers/IndexBuffer.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Mesh
	{
	public:
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex);
		~Mesh();

		void Bind() const;
		void Unbind() const;

		std::function<void(std::string_view, uint32_t)> SetTextureUniformCallback;

		uint32_t GetVertexCount() const;
		uint32_t GetIndexCount() const;
		uint32_t GetMaterialIndex() const;

	private:
		void InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

	private:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		const uint32_t m_materialIndex;

		Buffers::VertexArray m_vertexArray;
		std::unique_ptr<Buffers::VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Buffers::IndexBuffer> m_indexBuffer;
	};
}