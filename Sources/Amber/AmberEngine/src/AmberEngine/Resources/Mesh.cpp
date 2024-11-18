#include "Amberpch.h"

#include "AmberEngine/Resources/Mesh.h"

AmberEngine::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex) :
m_vertexCount(static_cast<uint32_t>(p_vertices.size())),
m_indicesCount(static_cast<uint32_t>(p_indices.size())),
m_materialIndex(p_materialIndex)
{
	InitBuffers(p_vertices, p_indices);
}

AmberEngine::Resources::Mesh::~Mesh()
{
	Mesh::Unbind();
}

void AmberEngine::Resources::Mesh::Bind()
{
	m_vertexArray.Bind();
}

void AmberEngine::Resources::Mesh::Unbind()
{
	m_vertexArray.Unbind();
}

uint32_t AmberEngine::Resources::Mesh::GetVertexCount()
{
	return m_vertexCount;
}

uint32_t AmberEngine::Resources::Mesh::GetIndexCount()
{
	return m_indicesCount;
}

uint32_t AmberEngine::Resources::Mesh::GetMaterialIndex() const
{
	return m_materialIndex;
}

void AmberEngine::Resources::Mesh::InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
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

	constexpr uint64_t vertexSize = sizeof(Geometry::Vertex);
	
	m_vertexBuffer = std::make_unique<Buffers::VertexBuffer>(vertexData.data(), vertexData.size());
	m_indexBuffer = std::make_unique<Buffers::IndexBuffer>(p_indices.data(), p_indices.size());
	
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, nullptr);
	m_vertexArray.BindAttribPointer(2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 3));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 5));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 8));
	m_vertexArray.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 11));

	m_vertexArray.Unbind();
}
