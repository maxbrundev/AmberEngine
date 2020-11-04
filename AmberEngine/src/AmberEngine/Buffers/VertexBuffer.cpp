#include "Amberpch.h"

#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Buffers::VertexBuffer::VertexBuffer(const void* p_data, unsigned int p_size)
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, p_size, p_data, GL_STATIC_DRAW));
}

AmberEngine::Buffers::VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void AmberEngine::Buffers::VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void AmberEngine::Buffers::VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}