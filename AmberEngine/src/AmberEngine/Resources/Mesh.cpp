#include "Amberpch.h"

#include "AmberEngine/Resources/Mesh.h"

AmberEngine::Resources::Mesh::Mesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, std::vector<Texture*> p_textures)
	:m_vertexCount(static_cast<uint32_t>(p_vertices.size())), m_indicesCount(static_cast<uint32_t>(p_indices.size())), m_textures(p_textures)
{
	InitBuffers(p_vertices, p_indices);
}

AmberEngine::Resources::Mesh::~Mesh()
{
	Unbind();
}

void AmberEngine::Resources::Mesh::Bind()
{
	m_vao.Bind();
}

void AmberEngine::Resources::Mesh::Unbind()
{
	m_vao.Unbind();
}

void AmberEngine::Resources::Mesh::BindMaterialTextures()
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i]->Bind(i);

		std::string_view name = m_textures[i]->m_type;
		
		if (name == "texture_diffuse")
		{
			SetTextureUniformCallback("u_DiffuseMap", i);
		}
		else if (name == "texture_specular")
		{
			SetTextureUniformCallback("u_SpecularMap", i);
		}
	}
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
	
	uint64_t vertexSize = sizeof(Geometry::Vertex);
	
	m_vbo = std::make_unique<Buffers::VertexBuffer>(vertexData.data(), vertexData.size());
	m_ebo = std::make_unique<Buffers::IndexBuffer>(p_indices.data(), p_indices.size());
	
	m_vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, static_cast<void*>(nullptr));
	m_vao.BindAttribPointer(2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 3));
	m_vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 5));
	m_vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 8));
	m_vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<GLvoid*>(sizeof(float) * 11));

	m_vao.Unbind();
}