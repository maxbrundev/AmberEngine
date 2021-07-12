#include "Amberpch.h"

#include "AmberEngine/Resources/Shader.h"

#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Resources::Shader::Shader(const std::string& p_path, uint32_t p_id) : path(p_path), id(p_id)
{
}

AmberEngine::Resources::Shader::~Shader()
{
	GLCall(glDeleteProgram(id));
}

void AmberEngine::Resources::Shader::Bind() const
{
	GLCall(glUseProgram(id));
}

void AmberEngine::Resources::Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void AmberEngine::Resources::Shader::SetUniformIntPointer(std::string_view p_name, int p_count, const int* p_value)
{
	GLCall(glUniform1iv(GetUniformLocation(p_name), p_count, p_value));
}

void AmberEngine::Resources::Shader::SetUniform1i(std::string_view p_name, const int p_value)
{
	GLCall(glUniform1i(GetUniformLocation(p_name), p_value));
}

void AmberEngine::Resources::Shader::SetUniform1f(std::string_view p_name, const float p_value)
{
	GLCall(glUniform1f(GetUniformLocation(p_name), p_value));
}

void AmberEngine::Resources::Shader::SetUniform4f(std::string_view p_name, const float p_v0, const float p_v1, const float p_v2, const float p_v3)
{
	GLCall(glUniform4f(GetUniformLocation(p_name), p_v0, p_v1, p_v2, p_v3));
}

void AmberEngine::Resources::Shader::SetUniformVec3(std::string_view p_name, const glm::vec3& p_vec3)
{
	GLCall(glUniform3f(GetUniformLocation(p_name), p_vec3.x, p_vec3.y, p_vec3.z));
}

void AmberEngine::Resources::Shader::SetUniformVec4(std::string_view p_name, const glm::vec4& p_vec4)
{
	GLCall(glUniform4f(GetUniformLocation(p_name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w));
}

void AmberEngine::Resources::Shader::SetUniformMat4(std::string_view p_name, const glm::mat4& p_mat4)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_FALSE, &p_mat4[0][0]));
}

uint32_t AmberEngine::Resources::Shader::GetUniformLocation(std::string_view p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	const uint32_t location = glGetUniformLocation(id, static_cast<std::string>(p_name).c_str());
	if (location == -1)
		std::cout << "warning uniform doesn't exist" << std::endl;

	m_uniformLocationCache[p_name] = location;

	return location;
}