#include "Amberpch.h"

#include "AmberEngine/Resources/AssimpMesh.h"

AmberEngine::Resources::AssimpMesh::AssimpMesh(std::vector<AssimpVertex> p_vertices, std::vector<unsigned int> p_indices, std::vector<Texture*> p_textures) 
	: m_vertices(p_vertices), m_textures(p_textures), m_indices(p_indices)
{
	InitBuffers();
}

void AmberEngine::Resources::AssimpMesh::BindBuffers(Shader& p_shader)
{	
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

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void AmberEngine::Resources::AssimpMesh::InitBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(AssimpVertex), &m_vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), static_cast<void*>(nullptr));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), reinterpret_cast<void*>(offsetof(AssimpVertex, texCoords)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), reinterpret_cast<void*>(offsetof(AssimpVertex, normal)));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), reinterpret_cast<void*>(offsetof(AssimpVertex, tangent)));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex), reinterpret_cast<void*>(offsetof(AssimpVertex, bitangent)));
	
	glBindVertexArray(0);
}

void AmberEngine::Resources::AssimpMesh::DeleteBuffers()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}
