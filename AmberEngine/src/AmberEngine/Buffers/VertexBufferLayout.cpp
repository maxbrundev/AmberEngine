#include "Amberpch.h"

#include "AmberEngine/Buffers/VertexBufferLayout.h"
#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Buffers::VertexBufferLayout::VertexBufferLayout() : m_stride(0)
{
}

void AmberEngine::Buffers::VertexBufferLayout::PushFloat(unsigned int count)
{
	m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	m_stride += count * sizeof(GL_FLOAT);
}

const std::vector<AmberEngine::Buffers::VertexBufferElement>& AmberEngine::Buffers::VertexBufferLayout::GetElements() const
{
	return m_elements;
}

unsigned AmberEngine::Buffers::VertexBufferLayout::GetStride() const
{
	return m_stride;
}