#include "Amberpch.h"

#include "AmberEngine/Managers/MaterialManager.h"

#include "AmberEngine/Resources/Loaders/MaterialLoader.h"

AmberEngine::Resources::Material* AmberEngine::ResourceManagement::MaterialManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	std::string realPath = GetRealPath(p_path);

	Resources::Material* material = Resources::Loaders::MaterialLoader::Create(isRealPathRequired ? realPath : p_path);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Resources::Material, path)) = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void AmberEngine::ResourceManagement::MaterialManager::DestroyResource(AmberEngine::Resources::Material* p_resource)
{
	Resources::Loaders::MaterialLoader::Destroy(p_resource);
}

void AmberEngine::ResourceManagement::MaterialManager::ReloadResource(Resources::Material* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	std::string realPath = GetRealPath(p_path);
	Resources::Loaders::MaterialLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path);
}
