#include "pch.h"

#include "Buffers/VertexArray.h"

RenderEngine::Buffers::VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererID));
}

RenderEngine::Buffers::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void RenderEngine::Buffers::VertexArray::AddBuffer(const VertexBuffer& p_vbo, const VertexBufferLayout& p_layout)
{
	Bind();
	p_vbo.Bind();
	const auto& elements = p_layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, p_layout.GetStride(), reinterpret_cast<const void*>(offset)));
		offset += element.count * sizeof(element.type);
	}
}

void RenderEngine::Buffers::VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_rendererID));
}

void RenderEngine::Buffers::VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}