#include "pch.h"

#include "Buffers/VertexBufferLayout.h"

RenderEngine::Buffers::VertexBufferLayout::VertexBufferLayout() : m_stride(0)
{
}

void RenderEngine::Buffers::VertexBufferLayout::PushFloat(unsigned int count)
{
	m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_stride += count * sizeof(GL_FLOAT);
}

const std::vector<RenderEngine::Buffers::VertexBufferElement>& RenderEngine::Buffers::VertexBufferLayout::GetElements() const
{
	return m_elements;
}

unsigned RenderEngine::Buffers::VertexBufferLayout::GetStride() const
{
	return m_stride;
}