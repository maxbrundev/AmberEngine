#include "Amberpch.h"

#include "AmberEngine/Resources/Loaders/ShaderLoader.h"

#include "AmberEngine/Tools/Utils/String.h"

#include "AmberEngine/Debug/GLDebug.h"

std::string AmberEngine::Resources::ShaderLoader::__FILE_TRACE;

AmberEngine::Resources::Shader* AmberEngine::Resources::ShaderLoader::Create(std::string p_filePath)
{
	__FILE_TRACE = p_filePath;

	const std::pair<std::string, std::string> source = ParseShader(p_filePath);

	const uint32_t programmeID = CreateShader(source.first, source.second);

	Shader* shader = new Shader(std::move(p_filePath), programmeID);

	return shader;
}

AmberEngine::Resources::Shader* AmberEngine::Resources::ShaderLoader::CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{
	__FILE_TRACE = Utils::String::ExtractDirectoryFromPath(p_vertexShader) + "/" + Utils::String::ExtractDirectoryFromPath(p_fragmentShader);

	const uint32_t programmeID = CreateShader(p_vertexShader, p_fragmentShader);

	std::string filePath = "";

	Shader* shader = new Shader(std::move(filePath), programmeID);

	return shader;
}

void AmberEngine::Resources::ShaderLoader::Recompile(Shader& p_shader, const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	std::pair<std::string, std::string> source = ParseShader(p_filePath);

	uint32_t programmeID = CreateShader(source.first, source.second);

	/* Pointer to the shaderID (const data member, tricks to access it) */
	uint32_t* shaderID = reinterpret_cast<uint32_t*>(&p_shader) + offsetof(Shader, id);

	/* Deletes the previous program */
	glDeleteProgram(*shaderID);

	/* Store the new program in the shader */
	*shaderID = programmeID;
}

bool AmberEngine::Resources::ShaderLoader::Destroy(Shader*& p_shader)
{
	if (p_shader)
	{
		delete p_shader;
		p_shader = nullptr;

		return true;
	}

	return false;
}

std::pair<std::string, std::string> AmberEngine::Resources::ShaderLoader::ParseShader(const std::string& p_filePath)
{
	std::ifstream stream(p_filePath);

	enum class ShaderType
	{
		NONE     = -1,
		VERTEX   = 0,
		FRAGMENT = 1
	};

	ShaderType shaderType = ShaderType::NONE;

	std::string currentLine;
	std::stringstream stringStream[2];

	while (getline(stream, currentLine))
	{
		if (currentLine.find("#shader") != std::string::npos)
		{
			if (currentLine.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::VERTEX;
			}
			else if (currentLine.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			stringStream[static_cast<int>(shaderType)] << currentLine << '\n';
		}
	}

	return 
	{ 
		stringStream[static_cast<int>(ShaderType::VERTEX)].str(),
		stringStream[static_cast<int>(ShaderType::FRAGMENT)].str()
	};
}

std::string AmberEngine::Resources::ShaderLoader::Parse(const std::string& p_filePath)
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

uint32_t AmberEngine::Resources::ShaderLoader::CreateShader(const std::string &p_vertexSources, const std::string& p_fragmentSources)
{
	GLCall(const uint32_t program = glCreateProgram());

	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, p_vertexSources);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, p_fragmentSources);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));

	GLint linkStatus;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

	if (linkStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

		std::cout << "[LINK] \"" + __FILE_TRACE + "\":\n" + errorLog << std::endl;

		GLCall(glDeleteProgram(program));

		return 0;
	}

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

uint32_t AmberEngine::Resources::ShaderLoader::CompileShader(uint32_t p_type, const std::string& p_source)
{
	GLCall(uint32_t id = glCreateShader(p_type));

	const char* src = p_source.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));

	GLCall(glCompileShader(id));

	GLint compileStatus;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));

	if (compileStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

		const std::string shaderTypeString = p_type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER";
		const std::string errorHeader = "[" + shaderTypeString + "] \"";

		std::cout << errorHeader + __FILE_TRACE + "\":\n" + errorLog << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}
