#include "Amberpch.h"

#include "AmberEngine/Resources/Shader.h"

#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Resources::Shader::Shader(const std::string& p_filePath) : m_sourceFile(p_filePath), m_rendererID(0)
{
	ShaderProgramSource source = ParseShader(p_filePath);
	m_rendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

AmberEngine::Resources::Shader::Shader(const std::string& p_vertexShader, const std::string& p_fragmentShader)
	: m_vertexFilePath(p_vertexShader), m_fragmentFilePath(p_fragmentShader), m_rendererID(0)
{
	const std::string vertexShaderSource = Parse(p_vertexShader);
	const std::string fragmentShaderSource = Parse(p_fragmentShader);

	m_rendererID = CreateShader(vertexShaderSource, fragmentShaderSource);
}

AmberEngine::Resources::Shader::Shader(const std::string& p_vertexShader, const std::string& p_geometryShader, const std::string& p_fragmentShader)
	:m_vertexFilePath(p_vertexShader), m_geometryFilePath(p_geometryShader), m_fragmentFilePath(p_fragmentShader), m_rendererID(0)
{
	const std::string vertexShaderSource = Parse(p_vertexShader);
	const std::string geometryShaderSource = Parse(p_geometryShader);
	const std::string fragmentShaderSource = Parse(p_fragmentShader);
	
	GLCall(const unsigned int program = glCreateProgram());

	const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	const unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSource);
	const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, gs));
	GLCall(glAttachShader(program, fs));
	
	GLCall(glLinkProgram(program));

	int link;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &link));
	if (link == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to link shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));

		return;
	}

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	GLCall(glDeleteShader(gs));

	m_rendererID = program;
}

AmberEngine::Resources::Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererID));
}

void AmberEngine::Resources::Shader::Bind() const
{
	GLCall(glUseProgram(m_rendererID));
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

std::string AmberEngine::Resources::Shader::Parse(const std::string& p_filePath) const
{
	std::ifstream stream(p_filePath);
	std::string line;
	std::stringstream stringstream;

	while (getline(stream, line))
	{
		stringstream << line << "\n";
	}

	return stringstream.str();
}

AmberEngine::Resources::ShaderProgramSource AmberEngine::Resources::Shader::ParseShader(const std::string& p_filePath)
{
	std::ifstream stream(p_filePath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int AmberEngine::Resources::Shader::CreateShader(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{

	GLCall(const unsigned int program = glCreateProgram());
	const unsigned int vs = CompileShader(GL_VERTEX_SHADER, p_vertexShader);
	const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));

	int link;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &link));
	if (link == GL_FALSE)
	{
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(program, length, &length, message));
		std::cout << "Failed to link shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteProgram(program));

		return 0;
	}

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int AmberEngine::Resources::Shader::CompileShader(unsigned int p_type, const std::string& p_source)
{
	GLCall(const unsigned int id = glCreateShader(p_type));
	const char* src = p_source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (p_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));

		return 0;
	}

	return id;
}

void AmberEngine::Resources::Shader::Recompile()
{
	LoadFromFile(m_sourceFile);
}

void AmberEngine::Resources::Shader::LoadFromFile(const std::string& p_path)
{
	m_sourceFile = p_path;
	ShaderProgramSource source = ParseShader(p_path);
	m_rendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

unsigned int AmberEngine::Resources::Shader::GetUniformLocation(std::string_view p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	const int location = glGetUniformLocation(m_rendererID, static_cast<std::string>(p_name).c_str());
	if (location == -1)
		std::cout << "warning uniform doesn't exist" << std::endl;

	m_uniformLocationCache[p_name] = location;

	return location;
}
