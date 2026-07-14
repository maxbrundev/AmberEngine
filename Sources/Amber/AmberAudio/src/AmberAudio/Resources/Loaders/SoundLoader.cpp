#include "Amberpch.h"

#include "AmberAudio/Resources/Loaders/SoundLoader.h"

AmberAudio::Resources::Sound* AmberAudio::Resources::Loaders::SoundLoader::Create(const std::string& p_filepath)
{
	return new Sound(p_filepath);
}

bool AmberAudio::Resources::Loaders::SoundLoader::Destroy(Sound*& p_soundInstance)
{
	if (p_soundInstance)
	{
		delete p_soundInstance;
		p_soundInstance = nullptr;

		return true;
	}

	return false;
}
