#include "Amberpch.h"

#include "AmberCore/ResourceManagement/MaterialManager.h"

#include "AmberCore/Resources/Loaders/MaterialLoader.h"

AmberCore::Resources::Material* AmberCore::ResourceManagement::MaterialManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	std::string realPath = GetRealPath(p_path);

	AmberCore::Resources::Material* material = AmberCore::Resources::Loaders::MaterialLoader::Create(isRealPathRequired ? realPath : p_path);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(AmberCore::Resources::Material, path)) = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void AmberCore::ResourceManagement::MaterialManager::DestroyResource(AmberCore::Resources::Material* p_resource)
{
	AmberCore::Resources::Loaders::MaterialLoader::Destroy(p_resource);
}

void AmberCore::ResourceManagement::MaterialManager::ReloadResource(AmberCore::Resources::Material* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	std::string realPath = GetRealPath(p_path);
	AmberCore::Resources::Loaders::MaterialLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path);
}
