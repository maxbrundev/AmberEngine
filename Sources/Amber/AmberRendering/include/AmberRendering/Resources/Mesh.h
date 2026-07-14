#pragma once


#include "AmberRendering/Geometry/BoundingSphere.h"
#include "AmberRendering/Geometry/Vertex.h"

#include "AmberRendering/Buffers/VertexArray.h"
#include "AmberRendering/Buffers/VertexBuffer.h"
#include "AmberRendering/Buffers/IndexBuffer.h"
#include "AmberRendering/Resources/IMesh.h"

namespace AmberRendering::Resources
{
	class Mesh : public IMesh
	{
	public:
		Mesh(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex);
		virtual ~Mesh() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		std::function<void(std::string_view, uint32_t)> SetTextureUniformCallback;

		virtual uint32_t GetVertexCount() override;
		virtual uint32_t GetIndexCount() override;

		uint32_t GetMaterialIndex() const;

		const AmberRendering::Geometry::BoundingSphere& GetBoundingSphere() const;

	private:
		void InitBuffers(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);
		void ComputeBoundingSphere(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices);

	private:
		const uint32_t m_vertexCount;
		const uint32_t m_indicesCount;
		const uint32_t m_materialIndex;

		AmberRendering::Buffers::VertexArray m_vertexArray;
		std::unique_ptr<AmberRendering::Buffers::VertexBuffer> m_vertexBuffer;
		std::unique_ptr<AmberRendering::Buffers::IndexBuffer> m_indexBuffer;

		AmberRendering::Geometry::BoundingSphere m_boundingSphere;
	};
}
