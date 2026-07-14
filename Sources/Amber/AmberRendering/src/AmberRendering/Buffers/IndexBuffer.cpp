#include "Amberpch.h"

#include "AmberRendering/Buffers/IndexBuffer.h"

AmberRendering::Buffers::IndexBuffer::IndexBuffer(const unsigned int* p_data, size_t p_elements)
{
	glGenBuffers(1, &m_bufferID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_elements * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
}

AmberRendering::Buffers::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void AmberRendering::Buffers::IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void AmberRendering::Buffers::IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t AmberRendering::Buffers::IndexBuffer::GetID() const
{
	return m_bufferID;
}
