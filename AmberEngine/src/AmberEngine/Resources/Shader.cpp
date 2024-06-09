#include "Amberpch.h"

#include "AmberEngine/Resources/Shader.h"

#include <GL/glew.h>

#include "AmberEngine/Resources/EUniformType.h"
#include "AmberEngine/Resources/Texture.h"

AmberEngine::Resources::Shader::Shader(std::string p_filePath, uint32_t p_id) : ID(p_id), Path(std::move(p_filePath))
{
	QueryUniforms();
}

AmberEngine::Resources::Shader::~Shader()
{
	glDeleteProgram(ID);
}

void AmberEngine::Resources::Shader::QueryUniforms()
{
	Uniforms.clear();

	GLint activeUniformsCount = 0;

	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &activeUniformsCount);

	std::vector<GLchar> nameData(256);

	for (int i = 0; i < activeUniformsCount; i++)
	{
		GLint arraySize      = 0;
		GLenum type          = 0;
		GLsizei length       = 0;

		glGetActiveUniform(ID, i, static_cast<GLsizei>(nameData.size()), &length, &arraySize, &type, &nameData[0]);

		const std::string uniformName((nameData.data()), length);

		if (!IsEngineUBOUniform(uniformName))
		{
			std::any defaultValue;

			switch (static_cast<EUniformType>(type))
			{
			case EUniformType::UNIFORM_BOOL:       defaultValue = std::make_any<bool>(GetUniformInt(uniformName));       break;
			case EUniformType::UNIFORM_INT:        defaultValue = std::make_any<int>(GetUniformInt(uniformName));        break;
			case EUniformType::UNIFORM_FLOAT:      defaultValue = std::make_any<float>(GetUniformFloat(uniformName));    break;
			case EUniformType::UNIFORM_FLOAT_VEC2: defaultValue = std::make_any<glm::vec2>(GetUniformVec2(uniformName)); break;
			case EUniformType::UNIFORM_FLOAT_VEC3: defaultValue = std::make_any<glm::vec3>(GetUniformVec3(uniformName)); break;
			case EUniformType::UNIFORM_FLOAT_VEC4: defaultValue = std::make_any<glm::vec4>(GetUniformVec4(uniformName)); break;
			case EUniformType::UNIFORM_SAMPLER_2D: defaultValue = std::make_any<Texture*>(nullptr);                      break;
			}

			if (defaultValue.has_value())
			{
				Uniforms.push_back
				({
					static_cast<EUniformType>(type),
					uniformName,
					GetUniformLocation(nameData.data()),
					defaultValue
					});
			}
		}
	}
}

void AmberEngine::Resources::Shader::Bind() const
{
	glUseProgram(ID);
}

void AmberEngine::Resources::Shader::Unbind() const
{
	glUseProgram(0);
}

void AmberEngine::Resources::Shader::SetUniformIntPointer(const std::string_view p_name, int p_count, const int* p_value)
{
	glUniform1iv(GetUniformLocation(p_name), p_count, p_value);
}

void AmberEngine::Resources::Shader::SetUniformInt(const std::string_view p_name, const int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void AmberEngine::Resources::Shader::SetUniformFloat(const std::string_view p_name, const float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void AmberEngine::Resources::Shader::SetUniform4Float(const std::string_view p_name, const float p_v0, const float p_v1, const float p_v2, const float p_v3)
{
	glUniform4f(GetUniformLocation(p_name), p_v0, p_v1, p_v2, p_v3);
}

void AmberEngine::Resources::Shader::SetUniformVec2(const std::string_view p_name, const glm::vec2& p_vec2)
{
	glUniform2f(GetUniformLocation(p_name), p_vec2.x, p_vec2.y);
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

int AmberEngine::Resources::Shader::GetUniformInt(const std::string& p_name)
{
	int value;
	glGetUniformiv(ID, GetUniformLocation(p_name), &value);
	return value;
}

float AmberEngine::Resources::Shader::GetUniformFloat(const std::string& p_name)
{
	float value;
	glGetUniformfv(ID, GetUniformLocation(p_name), &value);
	return value;
}

glm::vec2 AmberEngine::Resources::Shader::GetUniformVec2(const std::string& p_name)
{
	GLfloat values[2];
	glGetUniformfv(ID, GetUniformLocation(p_name), values);
	return reinterpret_cast<glm::vec2&>(values);
}

glm::vec3 AmberEngine::Resources::Shader::GetUniformVec3(const std::string& p_name)
{
	GLfloat values[3];
	glGetUniformfv(ID, GetUniformLocation(p_name), values);
	return reinterpret_cast<glm::vec3&>(values);
}

glm::vec4 AmberEngine::Resources::Shader::GetUniformVec4(const std::string& p_name)
{
	GLfloat values[4];
	glGetUniformfv(ID, GetUniformLocation(p_name), values);
	return reinterpret_cast<glm::vec4&>(values);
}

glm::mat4 AmberEngine::Resources::Shader::GetUniformMat4(const std::string& p_name)
{
	GLfloat values[16];
	glGetUniformfv(ID, GetUniformLocation(p_name), values);
	return reinterpret_cast<glm::mat4&>(values);
}

const AmberEngine::Resources::UniformInfo* AmberEngine::Resources::Shader::GetUniformInfo(const std::string& p_name) const
{
	auto predicate = [p_name](const UniformInfo& p_element)
	{
		return p_name == p_element.name;
	};

	const auto result = std::find_if(Uniforms.begin(), Uniforms.end(), predicate);

	if (result != Uniforms.end())
		return &*result;

	return nullptr;
}

bool AmberEngine::Resources::Shader::IsEngineUBOUniform(const std::string& p_uniformName)
{
	return p_uniformName.rfind("ubo_", 0) == 0;
}

uint32_t AmberEngine::Resources::Shader::GetUniformLocation(const std::string_view p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	const int32_t location = glGetUniformLocation(ID, static_cast<std::string>(p_name).c_str());
	if (location == -1)
		std::cout << "warning uniform doesn't exist" << std::endl;

	m_uniformLocationCache[p_name] = location;

	return location;
}
