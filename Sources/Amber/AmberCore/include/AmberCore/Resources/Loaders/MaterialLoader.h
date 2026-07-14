#pragma once

#include "AmberCore/Resources/Material.h"

namespace AmberCore::Resources::Loaders
{
	class MaterialLoader
	{
	public:
		MaterialLoader() = delete;
		static Material* Create(const std::string & p_path);
		static void Reload(Material& p_material, const std::string& p_path);
		static void Save(Material& p_material, const std::string& p_path);
		static bool Destroy(Material*& p_material);
	};
}
