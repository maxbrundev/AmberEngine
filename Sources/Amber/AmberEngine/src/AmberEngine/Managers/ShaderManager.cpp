#include "Amberpch.h"

#include "AmberEngine/Managers/ShaderManager.h"

#include "AmberEngine/Resources/Loaders/ShaderLoader.h"

AmberEngine::Resources::Shader* AmberEngine::ResourceManagement::ShaderManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto shader = Resources::Loaders::ShaderLoader::Create(isRealPathRequired ? realPath : p_path);

	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(Resources::Shader, Path)) = p_path; // Force the resource path to fit the given path

	return shader;
}

void AmberEngine::ResourceManagement::ShaderManager::DestroyResource(Resources::Shader* p_resource)
{
	Resources::Loaders::ShaderLoader::Destroy(p_resource);
}

void AmberEngine::ResourceManagement::ShaderManager::ReloadResource(Resources::Shader* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	Resources::Loaders::ShaderLoader::Recompile(*p_resource, p_path);
}
