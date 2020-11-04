#include "Amberpch.h"

#include "AmberEngine/Buffers/IndexBuffer.h"
#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Buffers::IndexBuffer::IndexBuffer(const unsigned int* p_data, unsigned int p_count) : m_count(p_count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_count * sizeof(unsigned int), p_data, GL_STATIC_DRAW));
}

AmberEngine::Buffers::IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void AmberEngine::Buffers::IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void AmberEngine::Buffers::IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}