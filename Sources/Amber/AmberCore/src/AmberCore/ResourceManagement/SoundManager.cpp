#include "Amberpch.h"

#include "AmberCore/ResourceManagement/SoundManager.h"

AmberAudio::Resources::Sound* AmberCore::ResourceManagement::SoundManager::CreateResource(const std::string& p_path, bool isRealPathRequired)
{
	const std::string realPath = GetRealPath(p_path);

	AmberAudio::Resources::Sound* sound = AmberAudio::Resources::Loaders::SoundLoader::Create(isRealPathRequired ? realPath : p_path);

	if (sound)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(sound) + offsetof(AmberAudio::Resources::Sound, Path)) = p_path; // Force the resource path to fit the given path

	return sound;
}

void AmberCore::ResourceManagement::SoundManager::DestroyResource(AmberAudio::Resources::Sound* p_resource)
{
	AmberAudio::Resources::Loaders::SoundLoader::Destroy(p_resource);
}

void AmberCore::ResourceManagement::SoundManager::ReloadResource(AmberAudio::Resources::Sound* p_resource, const std::string& p_path, bool isRealPathRequired)
{
}
