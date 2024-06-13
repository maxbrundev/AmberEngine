#pragma once

#include "AmberEngine/Managers/AResourceManager.h"

#include "AmberEngine/Resources/Model.h"

namespace AmberEngine::ResourceManagement
{
	class ModelManager : public AResourceManager<Resources::Model>
	{
	public:
		ModelManager() = default;
		virtual ~ModelManager() override = default;

		virtual Resources::Model* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(Resources::Model* p_resource) override;
		virtual void ReloadResource(Resources::Model* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
