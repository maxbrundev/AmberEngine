#include "Amberpch.h"

#include "AmberEngine/Resources/Loaders/ShaderLoader.h"

#include "AmberEngine/Tools/Utils/String.h"

std::string AmberEngine::Resources::Loaders::ShaderLoader::__FILE_TRACE;

AmberEngine::Resources::Shader* AmberEngine::Resources::Loaders::ShaderLoader::Create(std::string p_filePath)
{
	__FILE_TRACE = p_filePath;

	const std::pair<std::string, std::string> source = ParseShader(p_filePath);

	const uint32_t programID = CreateShader(source.first, source.second);

	Shader* shader = new Shader(std::move(p_filePath), programID);

	return shader;
}

AmberEngine::Resources::Shader* AmberEngine::Resources::Loaders::ShaderLoader::CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{
	__FILE_TRACE = Tools::Utils::String::ExtractDirectoryFromPath(p_vertexShader) + "/" + Tools::Utils::String::ExtractDirectoryFromPath(p_fragmentShader);

	const uint32_t programID = CreateShader(p_vertexShader, p_fragmentShader);

	std::string filePath;

	Shader* shader = new Shader(std::move(filePath), programID);

	return shader;
}

void AmberEngine::Resources::Loaders::ShaderLoader::Recompile(Shader& p_shader, const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	const std::pair<std::string, std::string> source = ParseShader(p_filePath);

	/* Create the new program */

	if (const uint32_t newProgramID = CreateShader(source.first, source.second))
	{
		/* Pointer to the shaderID (const data member, tricks to access it) */
		/* Only work with the first public member of a class */
		std::uint32_t* shaderID = reinterpret_cast<uint32_t*>(&p_shader) + offsetof(Shader, id);

		//uint32_t* shaderID = const_cast<uint32_t*>(&p_shader.id);

		/* Deletes the previous program */
		glDeleteProgram(*shaderID);

		/* Store the new program in the shader */
		*shaderID = newProgramID;

		p_shader.QueryUniforms();

		std::cout<< "[COMPILE] \"" + __FILE_TRACE + "\": Success!" << std::endl;
	}
	else
	{
		std::cout << "[COMPILE] \"" + __FILE_TRACE + "\": Failed! Previous shader version keept" << std::endl;
	}
}

bool AmberEngine::Resources::Loaders::ShaderLoader::Destroy(Shader*& p_shaderInstance)
{
	if (p_shaderInstance)
	{
		glDeleteProgram(p_shaderInstance->id);

		delete p_shaderInstance;
		p_shaderInstance = nullptr;

		return true;
	}

	return false;
}

bool AmberEngine::Resources::Loaders::ShaderLoader::Delete(Shader* p_shaderInstance)
{
	if (p_shaderInstance)
	{
		glDeleteProgram(p_shaderInstance->id);

		return true;
	}

	return false;
}

std::pair<std::string, std::string> AmberEngine::Resources::Loaders::ShaderLoader::ParseShader(const std::string& p_filePath)
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

std::string AmberEngine::Resources::Loaders::ShaderLoader::Parse(const std::string& p_filePath)
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

uint32_t AmberEngine::Resources::Loaders::ShaderLoader::CreateShader(const std::string &p_vertexSources, const std::string& p_fragmentSources)
{
	const uint32_t program = glCreateProgram();

	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, p_vertexSources, program);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, p_fragmentSources, program);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

		std::cout << "[LINK] \"" + __FILE_TRACE + "\":\n" + errorLog << std::endl;

		glDeleteProgram(program);

		glDetachShader(program, vs);
		glDetachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return 0;
	}

	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

uint32_t AmberEngine::Resources::Loaders::ShaderLoader::CompileShader(uint32_t p_type, const std::string& p_source, uint32_t p_program)
{
	uint32_t id = glCreateShader(p_type);

	const char* src = p_source.c_str();

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

		const std::string shaderTypeString = p_type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER";
		const std::string errorHeader = "[" + shaderTypeString + "] \"";

		std::cout << errorHeader + __FILE_TRACE + "\":\n" + errorLog << std::endl;

		glDetachShader(p_program, id);
		glDeleteShader(id);

		return 0;
	}

	return id;
}
