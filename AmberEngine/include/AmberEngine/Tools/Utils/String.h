#pragma once

#include "AmberEngine/API/Export.h"

namespace Tools::Utils
{
	class API_AMBERENGINE String
	{
	public:
		String() = delete;

		static std::string ExtractDirectoryFromPath(const std::string& p_path);
		static std::string ExtractFileNameFromPath(const std::string& p_path);
		static std::string RemoveExtensionFromFileName(const std::string& p_file);
		static bool Replace(std::string& p_target, const std::string& p_from, const std::string& p_to);
		static void ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to);
	};
}
