#pragma once

#include "AmberCore/ResourceManagement/AResourceManager.h"

#include "AmberRendering/Resources/Model.h"

namespace AmberCore::ResourceManagement
{
	class ModelManager : public AResourceManager<AmberRendering::Resources::Model>
	{
	public:
		ModelManager() = default;
		virtual ~ModelManager() override = default;

		virtual AmberRendering::Resources::Model* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberRendering::Resources::Model* p_resource) override;
		virtual void ReloadResource(AmberRendering::Resources::Model* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
