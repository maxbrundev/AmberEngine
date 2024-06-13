#include "Amberpch.h"

#include "AmberEngine/Managers/TextureManager.h"

#include "AmberEngine/Resources/Loaders/TextureLoader.h"

AmberEngine::Resources::Texture* AmberEngine::ResourceManagement::TextureManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	const auto texture = Resources::Loaders::TextureLoader::Create(isRealPathRequired ? realPath : p_path, Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR, Resources::ETextureFilteringMode::LINEAR, Resources::ETextureType::DIFFUSE_MAP,false, true);

	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(Resources::Texture, Path)) = p_path;

	return texture;
}

void AmberEngine::ResourceManagement::TextureManager::DestroyResource(Resources::Texture* p_resource)
{
	Resources::Loaders::TextureLoader::Destroy(p_resource);
}

void AmberEngine::ResourceManagement::TextureManager::ReloadResource(Resources::Texture* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	Resources::Loaders::TextureLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path, Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR, Resources::ETextureFilteringMode::LINEAR, Resources::ETextureType::DIFFUSE_MAP, false, true);
}
