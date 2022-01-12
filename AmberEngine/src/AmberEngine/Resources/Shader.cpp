#include "Amberpch.h"

#include "AmberEngine/Resources/Shader.h"

AmberEngine::Resources::Shader::Shader(std::string p_filePath, uint32_t p_id) : path(std::move(p_filePath)), id(p_id)
{
}

AmberEngine::Resources::Shader::~Shader()
{
	glDeleteProgram(id);
}

void AmberEngine::Resources::Shader::Bind() const
{
	glUseProgram(id);
}

void AmberEngine::Resources::Shader::Unbind() const
{
	glUseProgram(0);
}

void AmberEngine::Resources::Shader::SetUniformIntPointer(const std::string_view p_name, int p_count, const int* p_value)
{
	glUniform1iv(GetUniformLocation(p_name), p_count, p_value);
}

void AmberEngine::Resources::Shader::SetUniform1i(const std::string_view p_name, const int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void AmberEngine::Resources::Shader::SetUniform1f(const std::string_view p_name, const float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void AmberEngine::Resources::Shader::SetUniform4f(const std::string_view p_name, const float p_v0, const float p_v1, const float p_v2, const float p_v3)
{
	glUniform4f(GetUniformLocation(p_name), p_v0, p_v1, p_v2, p_v3);
}

void AmberEngine::Resources::Shader::SetUniformVec3(const std::string_view p_name, const glm::vec3& p_vec3)
{
	glUniform3f(GetUniformLocation(p_name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void AmberEngine::Resources::Shader::SetUniformVec4(const std::string_view p_name, const glm::vec4& p_vec4)
{
	glUniform4f(GetUniformLocation(p_name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w);
}

void AmberEngine::Resources::Shader::SetUniformMat4(const std::string_view p_name, const glm::mat4& p_mat4)
{
	glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_FALSE, &p_mat4[0][0]);
}

uint32_t AmberEngine::Resources::Shader::GetUniformLocation(const std::string_view p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	const int32_t location = glGetUniformLocation(id, static_cast<std::string>(p_name).c_str());
	if (location == -1)
		std::cout << "warning uniform doesn't exist" << std::endl;

	m_uniformLocationCache[p_name] = location;

	return location;
}