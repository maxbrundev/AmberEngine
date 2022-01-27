#include "Amberpch.h"

#include "AmberEngine/Resources/Mesh.h"

#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, const std::vector<std::shared_ptr<Texture>>& p_textures) :
m_vertexCount(p_vertices.size()),
m_indicesCount(p_indices.size()),
m_material(p_textures)
{
	InitBuffers(p_vertices, p_indices);
}

AmberEngine::Resources::Mesh::~Mesh()
{
	Unbind();
}

void AmberEngine::Resources::Mesh::Bind() const
{
	m_vertexArray.Bind();
}

void AmberEngine::Resources::Mesh::Unbind() const
{
	m_vertexArray.Unbind();
}

AmberEngine::Resources::Material& AmberEngine::Resources::Mesh::GetMaterial()
{
	return m_material;
}

uint32_t AmberEngine::Resources::Mesh::GetVertexCount() const
{
	return m_vertexCount;
}

uint32_t AmberEngine::Resources::Mesh::GetIndexCount() const
{
	return m_indicesCount;
}

void AmberEngine::Resources::Mesh::InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
{
	std::vector<float> vertexData;

	for (const auto& vertex : p_vertices)
	{
		vertexData.push_back(vertex.position[0]);
		vertexData.push_back(vertex.position[1]);
		vertexData.push_back(vertex.position[2]);

		vertexData.push_back(vertex.texCoords[0]);
		vertexData.push_back(vertex.texCoords[1]);

		vertexData.push_back(vertex.normals[0]);
		vertexData.push_back(vertex.normals[1]);
		vertexData.push_back(vertex.normals[2]);

		vertexData.push_back(vertex.tangent[0]);
		vertexData.push_back(vertex.tangent[1]);
		vertexData.push_back(vertex.tangent[2]);

		vertexData.push_back(vertex.bitangent[0]);
		vertexData.push_back(vertex.bitangent[1]);
		vertexData.push_back(vertex.bitangent[2]);
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
