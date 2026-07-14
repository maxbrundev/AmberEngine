#pragma once

#include "AmberCore/ResourceManagement/AResourceManager.h"

#include "AmberRendering/Resources/Shader.h"

namespace AmberCore::ResourceManagement
{
	class ShaderManager : public AResourceManager<AmberRendering::Resources::Shader>
	{
	public:
		ShaderManager() = default;
		virtual ~ShaderManager() override = default;

		virtual AmberRendering::Resources::Shader* CreateResource(const std::string& p_path, bool isRealPathRequired = true) override;
		virtual void DestroyResource(AmberRendering::Resources::Shader* p_resource) override;
		virtual void ReloadResource(AmberRendering::Resources::Shader* p_resource, const std::string& p_path, bool isRealPathRequired = true) override;
	};
}
