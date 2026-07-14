#pragma once

#include <string>

#include "AmberAudio/Resources/Sound.h"

namespace AmberAudio::Resources::Loaders
{
	class SoundLoader
	{
	public:
		SoundLoader() = delete;

		static Sound* Create(const std::string& p_filepath);
		static bool Destroy(Sound*& p_soundInstance);
	};
}
