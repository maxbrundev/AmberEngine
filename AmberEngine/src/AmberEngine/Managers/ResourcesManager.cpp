#include "Amberpch.h"

#include "AmberEngine/Managers/ResourcesManager.h"

AmberEngine::Resources::AssimpParser AmberEngine::Managers::ResourcesManager::__ASSIMP;

AmberEngine::Managers::ResourcesManager::ResourcesManager() : m_textureRootDir("res/textures/")
{
}

AmberEngine::Resources::Model& AmberEngine::Managers::ResourcesManager::LoadModel(std::string_view p_name, std::string_view p_fileName)
{
	if (m_modelResources.find(p_name) != m_modelResources.end())
		return *m_modelResources[p_name].get();
	
	Resources::Model* model = new Resources::Model(p_fileName);

	__ASSIMP.LoadModel(static_cast<std::string>(p_fileName), model->GetMeshes(), model->GetMaterialNames());
	
	const auto res = m_modelResources.emplace(p_name, model);
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShader(std::string_view p_name,
	const std::string& p_fileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	Resources::Shader* shader = AmberEngine::Resources::ShaderLoader::Create(p_fileName);

	const auto res = m_shaderResources.emplace(p_name, shader);
	return *res.first->second.get();
}

AmberEngine::Resources::Shader& AmberEngine::Managers::ResourcesManager::LoadShaderFiles(std::string_view p_name,
	const std::string& p_VertexFileName, const std::string& p_FragmentFileName)
{
	if (m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	Resources::Shader* shader = AmberEngine::Resources::ShaderLoader::CreateFromSource(p_VertexFileName, p_FragmentFileName);

	const auto res = m_shaderResources.emplace(p_name, shader);
	return *res.first->second.get();
}

AmberEngine::Resources::Texture& AmberEngine::Managers::ResourcesManager::LoadTexture(std::string_view p_name,
	const std::string& p_fileName, 
	AmberEngine::Settings::ETextureFilteringMode p_firstFilter,
	AmberEngine::Settings::ETextureFilteringMode p_secondFilter, 
	AmberEngine::Settings::ETextureType p_textureType,
	bool p_flipVertically, bool p_generateMipmap)
{
	if (m_TextureResources.find(p_name) != m_TextureResources.end())
		return *m_TextureResources[p_name].get();

	Resources::Texture* texture = AmberEngine::Resources::TextureLoader::Create(m_textureRootDir + p_fileName, p_firstFilter, p_secondFilter, p_textureType, p_flipVertically, p_generateMipmap);

	const auto res = m_TextureResources.emplace(p_name, texture);
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

AmberEngine::Resources::Model & AmberEngine::Managers::ResourcesManager::GetModel(std::string_view p_name)
{
	return *m_modelResources.at(p_name);
}

void AmberEngine::Managers::ResourcesManager::SetTextureRootDir(std::string_view p_directory)
{
	m_textureRootDir = p_directory;
}