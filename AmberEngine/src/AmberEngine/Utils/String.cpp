#include "Amberpch.h"

#include "AmberEngine/Utils/String.h"

std::string Utils::String::ExtractDirectoryFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	return p_path.substr(0, characterCount);
}

std::string Utils::String::ExtractFileNameFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	return p_path.substr(characterCount);
}

std::string Utils::String::RemoveExtensionFromFileName(const std::string& p_file)
{
	size_t characterCount = p_file.find_last_of('.');

	if(characterCount > 0 && characterCount != std::string::npos)
	{
		return p_file.substr(0, characterCount);
	}

	return p_file;
}
