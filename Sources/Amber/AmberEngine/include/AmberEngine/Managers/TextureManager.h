#pragma once

#include "AmberEngine/Managers/AResourceManager.h"

#include "AmberEngine/Resources/Texture.h"

namespace AmberEngine::ResourceManagement
{
	class TextureManager : public AResourceManager<Resources::Texture>
	{
	public:
		TextureManager() = default;
		virtual ~TextureManager() override = default;

		virtual Resources::Texture* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(Resources::Texture* p_resource) override;
		virtual void ReloadResource(Resources::Texture* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
