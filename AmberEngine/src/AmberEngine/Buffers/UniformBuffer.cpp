#include "Amberpch.h"

#include "AmberEngine/Buffers/UniformBuffer.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/Shader.h"

AmberEngine::Buffers::UniformBuffer::UniformBuffer(size_t p_size, uint32_t p_bindingPoint, uint32_t p_offset, EAccessSpecifier p_accessSpecifier)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
	glBufferData(GL_UNIFORM_BUFFER, p_size, nullptr, static_cast<GLint>(p_accessSpecifier));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, p_bindingPoint, m_bufferID, p_offset, p_size);
}

AmberEngine::Buffers::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void AmberEngine::Buffers::UniformBuffer::Bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
}

void AmberEngine::Buffers::UniformBuffer::Unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t AmberEngine::Buffers::UniformBuffer::GetID() const
{
	return m_bufferID;
}

void AmberEngine::Buffers::UniformBuffer::BindBlockToShader(Resources::Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint)
{
	glUniformBlockBinding(p_shader.id, p_uniformBlockLocation, p_bindingPoint);
}

void AmberEngine::Buffers::UniformBuffer::BindBlockToShader(Resources::Shader& p_shader, const std::string& p_name,
	uint32_t p_bindingPoint)
{
	glUniformBlockBinding(p_shader.id, GetBlockLocation(p_shader, p_name), p_bindingPoint);
}

uint32_t AmberEngine::Buffers::UniformBuffer::GetBlockLocation(Resources::Shader& p_shader, const std::string& p_name)
{
	return glGetUniformBlockIndex(p_shader.id, p_name.c_str());
}
