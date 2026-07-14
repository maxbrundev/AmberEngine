#pragma once

#include <string>

namespace AmberAudio::Resources
{
	namespace Loaders { class SoundLoader; }

	class Sound
	{
		friend class Loaders::SoundLoader;

	private:
		Sound(const std::string& p_path);

	public:
		const std::string Path;
	};
}
