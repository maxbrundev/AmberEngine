#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Geometry/Vertex.h"

#include "AmberEngine/Buffers/VertexArray.h"
#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Buffers/IndexBuffer.h"
#include "AmberEngine/Resources/IMesh.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Mesh : public IMesh
	{
	public:
		Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex);
		virtual ~Mesh() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		std::function<void(std::string_view, uint32_t)> SetTextureUniformCallback;

		virtual uint32_t GetVertexCount() override;
		virtual uint32_t GetIndexCount() override;

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
