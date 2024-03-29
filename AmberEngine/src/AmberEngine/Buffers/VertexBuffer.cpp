#include "Amberpch.h"

#include "AmberEngine/Buffers/VertexBuffer.h"

AmberEngine::Buffers::VertexBuffer::VertexBuffer(const void* p_data, size_t p_elements)
{
	glGenBuffers(1, &m_bufferID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, p_elements * sizeof(float), p_data, GL_STATIC_DRAW);
}

AmberEngine::Buffers::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void AmberEngine::Buffers::VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

void AmberEngine::Buffers::VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint AmberEngine::Buffers::VertexBuffer::GetID()
{
	return m_bufferID;
}
