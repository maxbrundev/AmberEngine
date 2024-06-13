#include "Amberpch.h"

#include "AmberEngine/Buffers/ShaderStorageBuffer.h"

AmberEngine::Buffers::ShaderStorageBuffer::ShaderStorageBuffer(EAccessSpecifier p_accessSpecifier)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, static_cast<GLenum>(p_accessSpecifier));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_bufferID);
}

AmberEngine::Buffers::ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void AmberEngine::Buffers::ShaderStorageBuffer::Bind(uint32_t p_bindingPoint)
{
	m_bindingPoint = p_bindingPoint;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, p_bindingPoint, m_bufferID);
}

void AmberEngine::Buffers::ShaderStorageBuffer::Unbind()
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, 0);
}
