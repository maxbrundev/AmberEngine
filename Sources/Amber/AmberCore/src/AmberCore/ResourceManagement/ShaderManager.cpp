#include "Amberpch.h"

#include "AmberCore/ResourceManagement/ShaderManager.h"

#include "AmberRendering/Resources/Loaders/ShaderLoader.h"

AmberRendering::Resources::Shader* AmberCore::ResourceManagement::ShaderManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto shader = AmberRendering::Resources::Loaders::ShaderLoader::Create(isRealPathRequired ? realPath : p_path);

	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(AmberRendering::Resources::Shader, Path)) = p_path; // Force the resource path to fit the given path

	return shader;
}

void AmberCore::ResourceManagement::ShaderManager::DestroyResource(AmberRendering::Resources::Shader* p_resource)
{
	AmberRendering::Resources::Loaders::ShaderLoader::Destroy(p_resource);
}

void AmberCore::ResourceManagement::ShaderManager::ReloadResource(AmberRendering::Resources::Shader* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	AmberRendering::Resources::Loaders::ShaderLoader::Recompile(*p_resource, p_path);
}
