#pragma once

#include "AmberEngine/API/Export.h"

namespace Utils
{
	class API_AMBERENGINE String
	{
	public:
		String() = delete;

		static std::string ExtractDirectoryFromPath(const std::string& p_path);
		static std::string ExtractFileNameFromPath(const std::string& p_path);
		static std::string RemoveExtensionFromFileName(const std::string& p_file);
		static int* StringToIntArray(const std::string& p_file);
		static void ParseInputIntoVector(const std::string& p_inputString, std::vector<std::string>& p_outVector);
		static std::string RemoveAllOcurrences(const std::string& p_target, const char p_character);
		static std::string ToUpper(const std::string& p_target);
		static std::string ToMinor(const std::string& p_target);

	public:
		static const char __BACKSLASH;
		static const char __FORWARDSLASH;
		static const char __DOT;

	};
}
