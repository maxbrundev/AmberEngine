#include "Amberpch.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/Mesh.h"

AmberEngine::Resources::Mesh::Mesh(const std::string& p_filePath) : m_sourceFile(p_filePath)
{
	InitBuffers(ObjLoader(p_filePath).LoadModel());
}

AmberEngine::Resources::Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(4, m_vbo);
}

void AmberEngine::Resources::Mesh::Draw()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void AmberEngine::Resources::Mesh::InitBuffers(const ObjModel& p_model)
{
	m_numIndices = static_cast<unsigned int>(p_model.indices.size());

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(4, m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_model.positionsIndices[0]) * p_model.positionsIndices.size(), &p_model.positionsIndices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_model.normalsIndices[0]) * p_model.normalsIndices.size(), &p_model.normalsIndices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_model.texCoordsIndices[0]) * p_model.texCoordsIndices.size(), &p_model.texCoordsIndices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_model.indices[0]) * p_model.indices.size(), &p_model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}