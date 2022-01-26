#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::Resources::Loaders
{
	class API_AMBERENGINE ShaderLoader
	{
	public:
		ShaderLoader() = delete;

		static Shader* Create(std::string p_filePath);
		static Shader* CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader);
		static void	Recompile(Shader& p_shader, const std::string& p_filePath);
		static bool Destroy(Shader*& p_shaderInstance);
		static bool Delete(Shader* p_shaderInstance);

	private:
		static std::pair<std::string, std::string> ParseShader(const std::string& p_filePath);
		static std::string Parse(const std::string& p_filePath);
		static uint32_t CreateShader(const std::string& p_vertexSource, const std::string& p_fragmentSource);
		static uint32_t CompileShader(uint32_t p_type, const std::string& p_source, uint32_t p_program);

		static std::string __FILE_TRACE;
	};
}