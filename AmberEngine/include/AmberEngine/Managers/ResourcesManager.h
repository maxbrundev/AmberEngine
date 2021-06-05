#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/AssimpModel.h"

#include "AmberEngine/Resources/AssimpParser.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE ResourcesManager
	{
	public:
		ResourcesManager();
		~ResourcesManager() = default;

		Resources::AssimpModel& LoadModel(std::string_view p_name, std::string_view p_fileName);

		Resources::Shader& LoadShader(std::string_view p_name, std::string_view p_fileName);
		Resources::Shader& LoadShaderFiles(std::string_view p_name, std::string_view p_VertexFileName, std::string_view p_FragmentFileName);

		Resources::Texture& LoadTexture(std::string_view p_name, std::string_view p_fileName);

		Resources::Shader& GetShader(std::string_view p_name);
		Resources::Texture& GetTexture(std::string_view p_name);
		Resources::AssimpModel& GetModel(std::string_view p_name);

		void SetShaderRootDir(std::string_view p_directory);
		void SetTextureRootDir(std::string_view p_directory);

	private:
		std::string m_shaderRootDir;
		std::string m_textureRootDir;

		std::unordered_map<std::string_view, std::shared_ptr<Resources::Shader>> m_shaderResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::Texture>> m_TextureResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::AssimpModel>> m_modelResources;

		static Resources::AssimpParser __ASSIMP;
	};
}