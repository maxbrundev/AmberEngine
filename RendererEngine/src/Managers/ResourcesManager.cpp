#include "pch.h"

#include "Managers/ResourcesManager.h"

RenderEngine::Managers::ResourcesManager::ResourcesManager() : m_shaderRootDir("res/shaders/")
{
}

RenderEngine::Resources::Shader& RenderEngine::Managers::ResourcesManager::LoadShader(const std::string& p_name,
	const std::string& p_fileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_unique<Resources::Shader>(m_shaderRootDir + p_fileName));
	return *res.first->second.get();
}

RenderEngine::Resources::Shader& RenderEngine::Managers::ResourcesManager::GetShader(const std::string& p_name)
{
	return *m_shaderResources.at(p_name);
}

void RenderEngine::Managers::ResourcesManager::SetShaderRootDir(const std::string& p_directory)
{
	m_shaderRootDir = p_directory;
}