#include "Amberpch.h"

#include "AmberEngine/Resources/AssimpMesh.h"

AmberEngine::Resources::AssimpMesh::AssimpMesh(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, std::vector<Texture*> p_textures)
	:m_vertexCount(static_cast<uint32_t>(p_vertices.size())), m_indicesCount(static_cast<uint32_t>(p_indices.size())), m_textures(p_textures)
{
	InitBuffers(p_vertices, p_indices);
}

AmberEngine::Resources::AssimpMesh::~AssimpMesh()
{
	DeleteBuffers();
}

void AmberEngine::Resources::AssimpMesh::Bind()
{
	glBindVertexArray(m_vao);
}

void AmberEngine::Resources::AssimpMesh::Unbind()
{
	glBindVertexArray(0);
}

void AmberEngine::Resources::AssimpMesh::BindMaterialTextures(Shader& p_shader)
{
	p_shader.Bind();
	
	for (int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i]->Bind(i);

		std::string_view name = m_textures[i]->m_type;
		
		if (name == "texture_diffuse")
		{
			p_shader.SetUniform1i("u_DiffuseMap", i);
		}
		else if (name == "texture_specular")
		{
			p_shader.SetUniform1i("u_SpecularMap", i);
		}
	}
}

void AmberEngine::Resources::AssimpMesh::InitBuffers(const std::vector<Geometry::Vertex>& p_vertices, const std::vector<uint32_t>& p_indices)
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
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_indices.size() * sizeof(unsigned int), p_indices.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), static_cast<void*>(nullptr));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), reinterpret_cast<GLvoid*>(sizeof(float) * 3));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), reinterpret_cast<GLvoid*>(sizeof(float) * 5));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), reinterpret_cast<GLvoid*>(sizeof(float) * 8));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertexSize), reinterpret_cast<GLvoid*>(sizeof(float) * 11));
	
	glBindVertexArray(0);
}

void AmberEngine::Resources::AssimpMesh::DeleteBuffers()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}
