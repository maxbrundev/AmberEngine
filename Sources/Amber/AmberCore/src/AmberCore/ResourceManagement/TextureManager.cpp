#include "Amberpch.h"

#include "AmberCore/ResourceManagement/TextureManager.h"

#include "AmberRendering/Resources/Loaders/TextureLoader.h"
#include "AmberTools/FileSystem/IniFile.h"

std::tuple<AmberRendering::Resources::ETextureFilteringMode, AmberRendering::Resources::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = AmberTools::FileSystem::IniFile(p_path + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(AmberRendering::Resources::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(AmberRendering::Resources::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<AmberRendering::Resources::ETextureFilteringMode>(min), static_cast<AmberRendering::Resources::ETextureFilteringMode>(mag), mipmap };
}

AmberRendering::Resources::Texture* AmberCore::ResourceManagement::TextureManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	auto[min, mag, mipmap] = GetAssetMetadata(realPath);

	const auto texture = AmberRendering::Resources::Loaders::TextureLoader::Create(isRealPathRequired ? realPath : p_path, min, mag, AmberRendering::Resources::ETextureType::DIFFUSE_MAP, true, mipmap);

	if (texture)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(texture) + offsetof(AmberRendering::Resources::Texture, Path)) = p_path;

	return texture;
}

void AmberCore::ResourceManagement::TextureManager::DestroyResource(AmberRendering::Resources::Texture* p_resource)
{
	AmberRendering::Resources::Loaders::TextureLoader::Destroy(p_resource);
}

void AmberCore::ResourceManagement::TextureManager::ReloadResource(AmberRendering::Resources::Texture* p_resource, const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	auto[min, mag, mipmap] = GetAssetMetadata(realPath);

	AmberRendering::Resources::Loaders::TextureLoader::Reload(*p_resource, isRealPathRequired ? realPath : p_path, min, mag, AmberRendering::Resources::ETextureType::DIFFUSE_MAP, true, mipmap);
}
