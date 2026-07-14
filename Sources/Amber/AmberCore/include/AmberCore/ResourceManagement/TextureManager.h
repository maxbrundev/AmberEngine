#pragma once

#include "AmberCore/ResourceManagement/AResourceManager.h"

#include "AmberRendering/Resources/Texture.h"

namespace AmberCore::ResourceManagement
{
	class TextureManager : public AResourceManager<AmberRendering::Resources::Texture>
	{
	public:
		TextureManager() = default;
		virtual ~TextureManager() override = default;

		virtual AmberRendering::Resources::Texture* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberRendering::Resources::Texture* p_resource) override;
		virtual void ReloadResource(AmberRendering::Resources::Texture* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
