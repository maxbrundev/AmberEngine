#include "Amberpch.h"

#include "AmberEngine/Managers/ResourcesManager.h"

#include "AmberEngine/Resources/Loaders/ModelLoader.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Resources/Loaders/TextureLoader.h"

//AmberEngine::Managers::ResourcesManager* AmberEngine::Managers::ResourcesManager::m_instance = nullptr;
//AmberEngine::Managers::ResourcesManager AmberEngine::Managers::ResourcesManager::m_instance;

//AmberEngine::Managers::ResourcesManager& AmberEngine::Managers::ResourcesManager::Instance()
//{
//	//static ResourcesManager m_instance;
//	//return m_instance;
//
//	if(m_instance == nullptr)
//	{
//		m_instance = new ResourcesManager();
//	} 
//	
//	return *m_instance;
//}

//void AmberEngine::Managers::ResourcesManager::Dispose()
//{
//	delete m_instance;
//	m_instance = nullptr;
//}

std::string AmberEngine::Managers::ResourcesManager::__PROJECT_ASSETS_PATH = "";
std::string AmberEngine::Managers::ResourcesManager::__ENGINE_ASSETS_PATH = "";

AmberEngine::Managers::ResourcesManager::ResourcesManager()
{
}

AmberEngine::Resources::Model& AmberEngine::Managers::ResourcesManager::LoadModel(const std::string_view p_name, const std::string& p_filePath)
{
	if (m_modelResources.find(p_name) != m_modelResources.end())
		return *m_modelResources[p_name];

	const std::string realPath = GetRealPath(p_filePath);

	auto model = Resources::Loaders::ModelLoader::Create(realPath);
	
	const auto res = m_modelResources.emplace(p_name, model);
	return *res.first->second;
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShader(const std::string_view p_name,
	const std::string& p_filePath)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name];

	const std::string realPath = GetRealPath(p_filePath);

	Resources::Shader* shader = Resources::Loaders::ShaderLoader::Create(realPath);

	const auto res = m_shaderResources.emplace(p_name, shader);
	return *res.first->second;
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShaderFiles(const std::string_view p_name,
	const std::string& p_VertexFilePath, const std::string& p_FragmentFilePath)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name];

	const std::string vertexRealPath   = GetRealPath(p_VertexFilePath);
	const std::string fragmentRealPath = GetRealPath(p_FragmentFilePath);

	Resources::Shader* shader = Resources::Loaders::ShaderLoader::CreateFromSource(vertexRealPath, fragmentRealPath);

	const auto res = m_shaderResources.emplace(p_name, shader);
	return *res.first->second;
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::LoadTexture(const std::string_view p_name, const std::string& p_filePath, Resources::ETextureFilteringMode p_firstFilter, Resources::ETextureFilteringMode p_secondFilter, Resources::ETextureType p_textureType, bool p_flipVertically, bool p_generateMipmap)
{
	if (m_TextureResources.find(p_name) != m_TextureResources.end())
		return *m_TextureResources[p_name];

	const std::string realPath = GetRealPath(p_filePath);

	Resources::Texture* texture = Resources::Loaders::TextureLoader::Create(realPath, p_firstFilter, p_secondFilter, p_textureType, p_flipVertically, p_generateMipmap);

	const auto res = m_TextureResources.emplace(p_name, texture);
	return *res.first->second;
}

void AmberEngine::Managers::ResourcesManager::ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath)
{
	__PROJECT_ASSETS_PATH = p_projectAssetsPath;
	__ENGINE_ASSETS_PATH  = p_engineAssetsPath;
}

std::string AmberEngine::Managers::ResourcesManager::GetRealPath(const std::string& p_path) const
{
	std::string result;

	if (p_path[0] == ':') // The path is an engine path
	{
		// Adding the engine path + removing the special character
		result = __ENGINE_ASSETS_PATH + std::string(p_path.data() + 1, p_path.data() + p_path.size());
	}
	else // The path is a project path
	{
		result = __PROJECT_ASSETS_PATH + p_path;
	}

	return result;
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::GetShader(const std::string_view p_name) const
{
	return *m_shaderResources.at(p_name);
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::GetTexture(const std::string_view p_name) const
{
	return  *m_TextureResources.at(p_name);
}

AmberEngine::Resources::Model & AmberEngine::Managers::ResourcesManager::GetModel(const std::string_view p_name) const
{
	return *m_modelResources.at(p_name);
}
