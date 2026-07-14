#include "Amberpch.h"

#include "AmberRendering/Resources/Mesh.h"

AmberRendering::Resources::Mesh::Mesh(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex) :
m_vertexCount(static_cast<uint32_t>(p_vertices.size())),
m_indicesCount(static_cast<uint32_t>(p_indices.size())),
m_materialIndex(p_materialIndex)
{
	InitBuffers(p_vertices, p_indices);
	ComputeBoundingSphere(p_vertices);
}

AmberRendering::Resources::Mesh::~Mesh()
{
	Mesh::Unbind();
}

void AmberRendering::Resources::Mesh::Bind()
{
	m_vertexArray.Bind();
}

void AmberRendering::Resources::Mesh::Unbind()
{
	m_vertexArray.Unbind();
}

uint32_t AmberRendering::Resources::Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t AmberRendering::Resources::Mesh::GetIndexCount()
{
	return m_indicesCount;
}

uint32_t AmberRendering::Resources::Mesh::GetMaterialIndex() const
{
	return m_materialIndex;
}

const AmberRendering::Geometry::BoundingSphere& AmberRendering::Resources::Mesh::GetBoundingSphere() const
{
	return m_boundingSphere;
}

void AmberRendering::Resources::Mesh::ComputeBoundingSphere(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices)
{
	m_boundingSphere.Position = glm::vec3(0.0f);
	m_boundingSphere.Radius = 0.0f;

	if (!p_vertices.empty())
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::lowest();
		float maxY = std::numeric_limits<float>::lowest();
		float maxZ = std::numeric_limits<float>::lowest();

		for (const auto& vertex : p_vertices)
		{
			minX = std::min(minX, vertex.Position[0]);
			minY = std::min(minY, vertex.Position[1]);
			minZ = std::min(minZ, vertex.Position[2]);

			maxX = std::max(maxX, vertex.Position[0]);
			maxY = std::max(maxY, vertex.Position[1]);
			maxZ = std::max(maxZ, vertex.Position[2]);
		}

		m_boundingSphere.Position = glm::vec3{ minX + maxX, minY + maxY, minZ + maxZ } / 2.0f;

		for (const auto& vertex : p_vertices)
		{
			const glm::vec3 position{ vertex.Position[0], vertex.Position[1], vertex.Position[2] };
			m_boundingSphere.Radius = std::max(m_boundingSphere.Radius, glm::distance(m_boundingSphere.Position, position));
		}
	}
}

void AmberRendering::Resources::Mesh::InitBuffers(const std::vector<AmberRendering::Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
{
	std::vector<float> vertexData;

	for (const auto& vertex : p_vertices)
	{
		vertexData.push_back(vertex.Position[0]);
		vertexData.push_back(vertex.Position[1]);
		vertexData.push_back(vertex.Position[2]);

		vertexData.push_back(vertex.TexCoords[0]);
		vertexData.push_back(vertex.TexCoords[1]);

		vertexData.push_back(vertex.Normals[0]);
		vertexData.push_back(vertex.Normals[1]);
		vertexData.push_back(vertex.Normals[2]);

		vertexData.push_back(vertex.Tangent[0]);
		vertexData.push_back(vertex.Tangent[1]);
		vertexData.push_back(vertex.Tangent[2]);

		vertexData.push_back(vertex.Bitangent[0]);
		vertexData.push_back(vertex.Bitangent[1]);
		vertexData.push_back(vertex.Bitangent[2]);
	}

	constexpr uint64_t vertexSize = sizeof(AmberRendering::Geometry::Vertex);
	
	m_vertexBuffer = std::make_unique<AmberRendering::Buffers::VertexBuffer>(vertexData.data(), vertexData.size());
	m_indexBuffer = std::make_unique<AmberRendering::Buffers::IndexBuffer>(p_indices.data(), p_indices.size());
	
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, nullptr);
	m_vertexArray.BindAttribPointer(2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 3));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 5));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 8));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 11));

	m_vertexArray.Unbind();
}
