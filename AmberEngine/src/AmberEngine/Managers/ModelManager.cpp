#include "Amberpch.h"

#include "AmberEngine/Managers/ModelManager.h"

#include "AmberEngine/Resources/Loaders/ModelLoader.h"

AmberEngine::Resources::Model* AmberEngine::ResourceManagement::ModelManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto model = Resources::Loaders::ModelLoader::Create(isRealPathRequired ? realPath : p_path);

	if (model)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(model) + offsetof(Resources::Model, Path)) = p_path; // Force the resource path to fit the given path

	return model;
}

void AmberEngine::ResourceManagement::ModelManager::DestroyResource(Resources::Model* p_resource)
{
	Resources::Loaders::ModelLoader::Destroy(p_resource);
}

void AmberEngine::ResourceManagement::ModelManager::ReloadResource(Resources::Model* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);
	Resources::Loaders::ModelLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path);
}
