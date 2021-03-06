#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	struct API_AMBERENGINE ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class API_AMBERENGINE Shader
	{
	private:
		std::unordered_map<std::string_view, int> m_uniformLocationCache;

		std::string m_vertexFilePath;
		std::string m_geometryFilePath;
		std::string m_fragmentFilePath;
			
		std::string m_sourceFile;

		GLuint m_programID;

	public:
		Shader() = default;
		Shader(const std::string& p_filePath);
		Shader(const std::string& p_vertexShader, const std::string& p_fragmentShader);
		Shader(const std::string& p_vertexShader, const std::string& p_geometryShader, const std::string& p_fragmentShader);

		~Shader();

		void Recompile();
		void LoadFromFile(const std::string& p_path);

		void Bind() const;
		void Unbind() const;

		void SetUniformIntPointer(std::string_view p_name, int p_count, const int* p_value);

		void SetUniform1i(std::string_view p_name, const int p_value);
		void SetUniform1f(std::string_view p_name, const float p_value);
		void SetUniform4f(std::string_view p_name, const float p_v0, const float p_v1, const float p_v2, const float p_v3);
		void SetUniformVec3(std::string_view p_name, const glm::vec3& p_vec3);
		void SetUniformVec4(std::string_view p_name, const glm::vec4& p_vec4);
		void SetUniformMat4(std::string_view p_name, const glm::mat4& p_mat4);

		inline unsigned int GetRendererID() const { return m_programID; }

	private:
		ShaderProgramSource ParseShader(const std::string& p_filePath);

		std::string Parse(const std::string& p_filePath) const;

		unsigned int CreateShader(const std::string& p_vertexShader, const std::string& p_fragmentShader);
		unsigned int CompileShader(unsigned int p_type, const std::string& p_source);
			
		unsigned int GetUniformLocation(std::string_view p_name);
	};
}