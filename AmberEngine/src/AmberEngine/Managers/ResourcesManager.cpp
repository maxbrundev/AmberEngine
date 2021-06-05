#include "Amberpch.h"

#include "AmberEngine/Managers/ResourcesManager.h"

AmberEngine::Resources::AssimpParser AmberEngine::Managers::ResourcesManager::__ASSIMP;

AmberEngine::Managers::ResourcesManager::ResourcesManager() : m_shaderRootDir("res/shaders/"), m_textureRootDir("res/textures/")
{
}

AmberEngine::Resources::AssimpModel& AmberEngine::Managers::ResourcesManager::LoadModel(std::string_view p_name, std::string_view p_fileName)
{
	if (m_modelResources.find(p_name) != m_modelResources.end())
		return *m_modelResources[p_name].get();
	
	Resources::AssimpModel* model = new Resources::AssimpModel(p_fileName);

	__ASSIMP.LoadModel(static_cast<std::string>(p_fileName), model->GetMeshes(), model->GetMaterialNames());
	
	const auto res = m_modelResources.emplace(p_name, model);
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShader(std::string_view p_name,
	std::string_view p_fileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_shared<Resources::Shader>(m_shaderRootDir + static_cast<std::string>(p_fileName)));
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShaderFiles(std::string_view p_name,
	std::string_view p_VertexFileName, std::string_view p_FragmentFileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_shared<Resources::Shader>(m_shaderRootDir + static_cast<std::string>(p_VertexFileName), m_shaderRootDir + static_cast<std::string>(p_FragmentFileName)));
	return *res.first->second.get();
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::LoadTexture(std::string_view p_name,
	std::string_view p_fileName)
{
	if (m_TextureResources.find(p_name) != m_TextureResources.end())
		return *m_TextureResources[p_name].get();

	const auto res = m_TextureResources.emplace(p_name, std::make_shared<Resources::Texture>(m_textureRootDir + static_cast<std::string>(p_fileName)));
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::GetShader(std::string_view p_name)
{
	return *m_shaderResources.at(p_name);
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::GetTexture(std::string_view p_name)
{
	return  *m_TextureResources.at(p_name);
}

AmberEngine::Resources::AssimpModel & AmberEngine::Managers::ResourcesManager::GetModel(std::string_view p_name)
{
	return *m_modelResources.at(p_name);
}

void AmberEngine::Managers::ResourcesManager::SetShaderRootDir(std::string_view p_directory)
{
	m_shaderRootDir = p_directory;
}

void AmberEngine::Managers::ResourcesManager::SetTextureRootDir(std::string_view p_directory)
{
	m_textureRootDir = p_directory;
}