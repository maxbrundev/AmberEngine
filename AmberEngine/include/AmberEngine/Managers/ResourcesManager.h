#pragma once

#include <unordered_map>

#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/Texture.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE ResourcesManager
	{
	private:
		std::string m_shaderRootDir;
		std::string m_textureRootDir;

		std::unordered_map<std::string, std::shared_ptr<Resources::Shader>> m_shaderResources;
		std::unordered_map<std::string, std::shared_ptr<Resources::Texture>> m_TextureResources;

	public:
		ResourcesManager();
		~ResourcesManager() = default;

		Resources::Shader& LoadShader(const std::string& p_name, const std::string& p_fileName);
		Resources::Shader& LoadShaderFiles(const std::string& p_name, const std::string& p_VertexFileName, const std::string& p_FragmentFileName);

		Resources::Texture& LoadTexture(const std::string& p_name, const std::string& p_fileName);

		Resources::Shader& GetShader(const std::string& p_name);
		Resources::Texture& GetTexture(const std::string& p_name);

		void SetShaderRootDir(const std::string& p_directory);
		void SetTextureRootDir(const std::string& p_directory);
	};
}