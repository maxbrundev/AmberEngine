#include "Amberpch.h"

#include "AmberEngine/Buffers/IndexBuffer.h"

AmberEngine::Buffers::IndexBuffer::IndexBuffer(const unsigned int* p_data, size_t p_elements)
{
	glGenBuffers(1, &m_bufferID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_elements * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
}

AmberEngine::Buffers::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void AmberEngine::Buffers::IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void AmberEngine::Buffers::IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t AmberEngine::Buffers::IndexBuffer::GetID() const
{
	return m_bufferID;
}
