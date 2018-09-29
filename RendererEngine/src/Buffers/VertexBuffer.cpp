#include "pch.h"

#include "Buffers/VertexBuffer.h"

RenderEngine::Buffers::VertexBuffer::VertexBuffer(const void* p_data, unsigned int p_size)
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, p_size, p_data, GL_STATIC_DRAW));
}

RenderEngine::Buffers::VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void RenderEngine::Buffers::VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void RenderEngine::Buffers::VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}