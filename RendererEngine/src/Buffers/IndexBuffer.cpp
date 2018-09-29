#include "pch.h"

#include "Buffers/IndexBuffer.h"

RenderEngine::Buffers::IndexBuffer::IndexBuffer(const unsigned int* p_data, unsigned int p_count) : m_count(p_count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_count * sizeof(unsigned int), p_data, GL_STATIC_DRAW));
}

RenderEngine::Buffers::IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void RenderEngine::Buffers::IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void RenderEngine::Buffers::IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}