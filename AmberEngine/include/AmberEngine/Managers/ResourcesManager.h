#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Resources/Texture.h"
#include "AmberEngine/Resources/Shader.h"
#include "AmberEngine/Resources/Model.h"

#include "AmberEngine/Tools/Utils/ASingleton.h"

namespace AmberEngine::Managers
{
	// ASingleton will be removed, just an experimentation purpose.
	class API_AMBERENGINE ResourcesManager final : public ASingleton<ResourcesManager>
	{
	public:
		friend class ASingleton<ResourcesManager>;

		//static ResourcesManager& Instance();
		//static void Dispose();
		//ResourcesManager(const ResourcesManager&) = delete;
		//ResourcesManager operator=(const ResourcesManager&) = delete;

		Resources::Model& LoadModel(const std::string_view p_name, const std::string& p_filePath);

		Resources::Shader& LoadShader(const std::string_view p_name, const std::string& p_filePath);
		Resources::Shader& LoadShaderFiles(const std::string_view p_name, const std::string& p_VertexFilePath, const std::string& p_FragmentFilePath);

		Resources::Texture& LoadTexture(const std::string_view p_name, const std::string& p_filePath, Resources::ETextureFilteringMode p_firstFilter, Resources::ETextureFilteringMode p_secondFilter, Resources::ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap);

		static void ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath);

		std::string GetRealPath(const std::string& p_path) const;

		[[nodiscard]] Resources::Shader& GetShader(const std::string_view p_name) const;
		[[nodiscard]] Resources::Texture& GetTexture(const std::string_view p_name) const;
		[[nodiscard]] Resources::Model& GetModel(const std::string_view p_name) const;

	private:
		ResourcesManager();
		~ResourcesManager() override = default;

	public:
		static std::string __PROJECT_ASSETS_PATH;
		static std::string __ENGINE_ASSETS_PATH;

		std::unordered_map<std::string_view, std::shared_ptr<Resources::Shader>> m_shaderResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::Texture>> m_TextureResources;
		std::unordered_map<std::string_view, std::shared_ptr<Resources::Model>> m_modelResources;
		//static ResourcesManager* m_instance;
		//static ResourcesManager m_instance;
	};
}
