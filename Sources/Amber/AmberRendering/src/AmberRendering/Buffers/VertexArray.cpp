#include "Amberpch.h"

#include "AmberRendering/Buffers/VertexArray.h"

AmberRendering::Buffers::VertexArray::VertexArray() : index{0}
{
	glGenVertexArrays(1, &m_bufferID);
	Bind();
}

AmberRendering::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void AmberRendering::Buffers::VertexArray::BindAttribPointer(const unsigned int p_size, const unsigned int p_type, const unsigned int p_normalized, const unsigned int p_stride, void* p_pointer)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(p_size), static_cast<GLenum>(p_type), p_normalized, static_cast<GLsizei>(p_stride), static_cast<const GLvoid*>(p_pointer));
	index++;
}

void AmberRendering::Buffers::VertexArray::Bind() const
{
	glBindVertexArray(m_bufferID);
}

void AmberRendering::Buffers::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

GLuint AmberRendering::Buffers::VertexArray::GetID() const
{
	return m_bufferID;
}
