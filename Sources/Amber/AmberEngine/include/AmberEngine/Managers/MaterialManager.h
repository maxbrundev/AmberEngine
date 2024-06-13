#pragma once
#include "AmberEngine/Managers/AResourceManager.h"
#include "AmberEngine/Resources/Material.h"

namespace AmberEngine::ResourceManagement
{
	class MaterialManager : public AResourceManager<AmberEngine::Resources::Material>
	{
	public:
		MaterialManager() = default;
		virtual ~MaterialManager() override = default;

		virtual Resources::Material* CreateResource(const std::string & p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberEngine::Resources::Material* p_resource) override;
		virtual void ReloadResource(AmberEngine::Resources::Material* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
