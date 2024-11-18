#include "Amberpch.h"

#include "AmberEngine/Managers/TextureManager.h"

#include "AmberEngine/Resources/Loaders/TextureLoader.h"
#include "AmberEngine/Tools/FileSystem/IniFile.h"

std::tuple<AmberEngine::Resources::ETextureFilteringMode, AmberEngine::Resources::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = AmberEngine::Tools::Filesystem::IniFile(p_path + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(AmberEngine::Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(AmberEngine::Resources::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<AmberEngine::Resources::ETextureFilteringMode>(min), static_cast<AmberEngine::Resources::ETextureFilteringMode>(mag), mipmap };
}

AmberEngine::Resources::Texture* AmberEngine::ResourceManagement::TextureManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	auto[min, mag, mipmap] = GetAssetMetadata(realPath);

	const auto texture = Resources::Loaders::TextureLoader::Create(isRealPathRequired ? realPath : p_path, min, mag, Resources::ETextureType::DIFFUSE_MAP,false, mipmap);

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

	auto[min, mag, mipmap] = GetAssetMetadata(realPath);

	Resources::Loaders::TextureLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path, min, mag, Resources::ETextureType::DIFFUSE_MAP, false, mipmap);
}
