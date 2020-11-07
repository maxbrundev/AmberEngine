#include "Amberpch.h"

#include <GL/glew.h>

#include "AmberEngine/Buffers/VertexArray.h"

#include "AmberEngine/Buffers/VertexBuffer.h"
#include "AmberEngine/Buffers/VertexBufferLayout.h"

AmberEngine::Buffers::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
}

AmberEngine::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void AmberEngine::Buffers::VertexArray::AddBuffer(const VertexBuffer& p_vbo, const VertexBufferLayout& p_layout)
{
	Bind();
	p_vbo.Bind();
	const auto& elements = p_layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(static_cast<GLuint>(i), static_cast<GLint>(element.count), static_cast<GLenum>(element.type), element.normalized, static_cast<GLsizei>(p_layout.GetStride()), reinterpret_cast<const GLvoid*>(offset));
		offset += element.count * sizeof(element.type);
	}
}

void AmberEngine::Buffers::VertexArray::Bind() const
{
	glBindVertexArray(m_bufferID);
}

void AmberEngine::Buffers::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

GLuint AmberEngine::Buffers::VertexArray::GetID()
{
	return m_bufferID;
}
