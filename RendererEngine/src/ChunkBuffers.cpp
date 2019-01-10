#include "pch.h"

#include "ChunkBuffers.h"
#include "BlockGeometry.h"

AmberCraft::ChunkBuffers::ChunkBuffers()
{
	InitBuffers();
}

void AmberCraft::ChunkBuffers::InitBuffers()
{
	AmberCraft::BlockGeometry::Setup();

	auto& vertices = AmberCraft::BlockGeometry::GetVertices();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AmberCraft::BlockVertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AmberCraft::BlockVertex), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AmberCraft::BlockVertex), reinterpret_cast<void*>(offsetof(AmberCraft::BlockVertex, textureCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AmberCraft::BlockVertex), reinterpret_cast<void*>(offsetof(AmberCraft::BlockVertex, normals)));

	glGenBuffers(1, &m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void AmberCraft::ChunkBuffers::SendBlocksToGPU(const std::vector<GLuint>& p_blocksToRender)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, p_blocksToRender.size() * sizeof(GLuint), p_blocksToRender.data(), GL_DYNAMIC_DRAW);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p, p_blocksToRender.data(), p_blocksToRender.size() * sizeof(GLuint));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void AmberCraft::ChunkBuffers::DrawChunk(uint16_t p_blocksToRenderCount)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);

	glBindVertexArray(m_vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, static_cast<GLsizei>(p_blocksToRenderCount));
	glBindVertexArray(0);
}