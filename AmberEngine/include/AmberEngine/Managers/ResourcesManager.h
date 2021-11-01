#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Resources/Parsers/AssimpParser.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE ResourcesManager
	{
	public:
		ResourcesManager();
		~ResourcesManager() = default;

		Resources::Model& LoadModel(const std::string_view p_name, const std::string& p_filePath);

		Resources::Shader& LoadShader(const std::string_view p_name, const std::string& p_filePath);
		Resources::Shader& LoadShaderFiles(const std::string_view p_name, const std::string& p_VertexFilePath, const std::string& p_FragmentFilePath);

		Resources::Texture& LoadTexture(const std::string_view p_name,
			const std::string& p_filePath, AmberEngine::Settings::ETextureFilteringMode p_firstFilter,
			AmberEngine::Settings::ETextureFilteringMode p_secondFilter, AmberEngine::Settings::ETextureType p_textureType,
			bool p_flipVertically, bool p_generateMipmap);

		Resources::Shader& GetShader(const std::string_view p_name);
		Resources::Texture& GetTexture(const std::string_view p_name);
		Resources::Model& GetModel(const std::string_view p_name);

		void SetTextureRootDir(const std::string_view p_directory);
		
	private:
		std::string m_textureRootDir;

		std::unordered_map<std::string_view, std::shared_ptr<Resources::Shader>> m_shaderResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::Texture>> m_TextureResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::Model>> m_modelResources;

		static Resources::AssimpParser __ASSIMP;
	};
}
