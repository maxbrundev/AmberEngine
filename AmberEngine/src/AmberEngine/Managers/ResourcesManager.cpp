#include <memory>

#include "AmberEngine/Managers/ResourcesManager.h"

AmberEngine::Managers::ResourcesManager::ResourcesManager() : m_shaderRootDir("res/shaders/"), m_textureRootDir("res/textures/")
{
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShader(const std::string& p_name,
	const std::string& p_fileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_shared<Resources::Shader>(m_shaderRootDir + p_fileName));
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShaderFiles(const std::string& p_name,
	const std::string& p_VertexFileName, const std::string& p_FragmentFileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_shared<Resources::Shader>(m_shaderRootDir + p_VertexFileName, m_shaderRootDir + p_FragmentFileName));
	return *res.first->second.get();
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::LoadTexture(const std::string& p_name,
	const std::string& p_fileName)
{
	if (m_TextureResources.find(p_name) != m_TextureResources.end())
		return *m_TextureResources[p_name].get();

	const auto res = m_TextureResources.emplace(p_name, std::make_shared<Resources::Texture>(m_textureRootDir + p_fileName));
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::GetShader(const std::string& p_name)
{
	return *m_shaderResources.at(p_name);
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::GetTexture(const std::string& p_name)
{
	return  *m_TextureResources.at(p_name);
}

void AmberEngine::Managers::ResourcesManager::SetShaderRootDir(const std::string& p_directory)
{
	m_shaderRootDir = p_directory;
}

void AmberEngine::Managers::ResourcesManager::SetTextureRootDir(const std::string& p_directory)
{
	m_textureRootDir = p_directory;
}