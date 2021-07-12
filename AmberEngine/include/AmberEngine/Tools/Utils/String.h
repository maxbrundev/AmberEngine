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
		static int* StringIntToArray(const std::string& p_file);
		static void ParseInputIntoVector(const std::string& p_inputString, std::vector<std::string>& p_outVector);
	};
}
