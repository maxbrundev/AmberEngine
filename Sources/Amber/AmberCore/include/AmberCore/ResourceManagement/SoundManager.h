#pragma once

#include "AmberAudio/Resources/Loaders/SoundLoader.h"

#include "AmberCore/ResourceManagement/AResourceManager.h"

namespace AmberCore::ResourceManagement
{
	class SoundManager : public AResourceManager<AmberAudio::Resources::Sound>
	{
	public:
		virtual AmberAudio::Resources::Sound* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberAudio::Resources::Sound* p_resource) override;
		virtual void ReloadResource(AmberAudio::Resources::Sound* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
