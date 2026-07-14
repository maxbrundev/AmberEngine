#pragma once
#include "AmberCore/ResourceManagement/AResourceManager.h"
#include "AmberCore/Resources/Material.h"

namespace AmberCore::ResourceManagement
{
	class MaterialManager : public AResourceManager<AmberCore::Resources::Material>
	{
	public:
		MaterialManager() = default;
		virtual ~MaterialManager() override = default;

		virtual AmberCore::Resources::Material* CreateResource(const std::string & p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberCore::Resources::Material* p_resource) override;
		virtual void ReloadResource(AmberCore::Resources::Material* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
