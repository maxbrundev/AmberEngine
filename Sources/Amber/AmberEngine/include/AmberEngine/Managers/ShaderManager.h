#pragma once

#include "AmberEngine/Managers/AResourceManager.h"

#include "AmberEngine/Resources/Shader.h"

namespace AmberEngine::ResourceManagement
{
	class ShaderManager : public AResourceManager<Resources::Shader>
	{
	public:
		ShaderManager() = default;
		virtual ~ShaderManager() override = default;

		virtual Resources::Shader* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(Resources::Shader* p_resource) override;
		virtual void ReloadResource(Resources::Shader* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
