#pragma once

#include "AmberEngine/Resources/Material.h"

namespace AmberEngine::Resources::Loaders
{
	class API_AMBERENGINE MaterialLoader
	{
	public:
		MaterialLoader() = delete;
		static Material* Create(const std::string & p_path);
		static void Reload(Material& p_material, const std::string& p_path);
		static void Save(Material& p_material, const std::string& p_path);
		static bool Destroy(Material*& p_material);
	};
}
