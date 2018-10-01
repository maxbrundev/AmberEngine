#pragma once

#include "Resources/Shader.h"

namespace RenderEngine
{
	namespace Managers
	{
		class ResourcesManager
		{
		private:
			std::string m_shaderRootDir;

			std::unordered_map<std::string, std::unique_ptr<Resources::Shader>> m_shaderResources;

		public:
			ResourcesManager();
			~ResourcesManager() = default;

			Resources::Shader& LoadShader(const std::string& p_name, const std::string& p_fileName);
			Resources::Shader& LoadShaderFiles(const std::string& p_name, const std::string& p_VertexFileName, const std::string& p_FragmentFileName);

			Resources::Shader& GetShader(const std::string& p_name);
			void SetShaderRootDir(const std::string& p_directory);
		};
	}
}