#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/String.h"

std::string AmberEngine::Tools::Utils::String::ExtractDirectoryFromPath(const std::string& p_path)
{
	size_t end_pos = p_path.find_last_of('/') + 1;

	end_pos = end_pos > 0 ? end_pos :  p_path.find_last_of('\\') + 1;

	return p_path.substr(0, end_pos);
}

std::string AmberEngine::Tools::Utils::String::ExtractFileNameFromPath(const std::string& p_path)
{
	size_t characterCount = p_path.find_last_of('/') + 1;

	characterCount = characterCount > 0 ? characterCount : p_path.find_last_of('\\') + 1;

	return p_path.substr(characterCount);
}

std::string AmberEngine::Tools::Utils::String::RemoveExtensionFromFileName(const std::string& p_file)
{
	const size_t end_pos = p_file.find_last_of('.');

	if(end_pos > 0 && end_pos != std::string::npos)
	{
		return p_file.substr(0, end_pos);
	}

	return p_file;
}

bool AmberEngine::Tools::Utils::String::Replace(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
	const size_t start_pos = p_target.find(p_from);

	if (start_pos != std::string::npos)
	{
		p_target.replace(start_pos, p_from.length(), p_to);
		return true;
	}

	return false;
}

void AmberEngine::Tools::Utils::String::ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
	if (p_from.empty()) return;

	size_t start_pos = 0;
	while ((start_pos = p_target.find(p_from, start_pos)) != std::string::npos)
	{
		p_target.replace(start_pos, p_from.length(), p_to);
		start_pos += p_to.length();
	}
}