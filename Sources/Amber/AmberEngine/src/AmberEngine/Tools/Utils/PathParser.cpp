#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/PathParser.h"

std::string AmberEngine::Tools::Utils::PathParser::MakeWindowsStyle(const std::string& p_path)
{
	std::string result;
	result.resize(p_path.size());

	for (size_t i = 0; i < p_path.size(); ++i)
		result[i] = p_path[i] == '/' ? '\\' : p_path[i];

	return result;
}

std::string AmberEngine::Tools::Utils::PathParser::MakeNonWindowsStyle(const std::string& p_path)
{
	std::string result;
	result.resize(p_path.size());

	for (size_t i = 0; i < p_path.size(); ++i)
		result[i] = p_path[i] == '\\' ? '/' : p_path[i];

	return result;
}

std::string AmberEngine::Tools::Utils::PathParser::GetContainingFolder(const std::string& p_path)
{
	std::string result;

	bool extraction = false;

	for (auto it = p_path.rbegin(); it != p_path.rend(); ++it)
	{
		if (extraction)
			result += *it;

		if (!extraction && it != p_path.rbegin() && (*it == '\\' || *it == '/'))
			extraction = true;
	}

	std::reverse(result.begin(), result.end());

	if (!result.empty() && result.back() != '\\')
		result += '\\';

	return result;
}

std::string AmberEngine::Tools::Utils::PathParser::GetElementName(const std::string& p_path)
{
	std::string result;

	std::string path = p_path;
	if (!path.empty() && path.back() == '\\')
		path.pop_back();

	for (auto it = path.rbegin(); it != path.rend() && *it != '\\' && *it != '/'; ++it)
		result += *it;

	std::reverse(result.begin(), result.end());

	return result;
}

std::string AmberEngine::Tools::Utils::PathParser::GetExtension(const std::string& p_path)
{
	std::string result;

	for (auto it = p_path.rbegin(); it != p_path.rend() && *it != '.'; ++it)
		result += *it;

	std::reverse(result.begin(), result.end());

	return result;
}

std::string AmberEngine::Tools::Utils::PathParser::FileTypeToString(EFileType p_fileType)
{
	switch (p_fileType)
	{
	case AmberEngine::Tools::Utils::PathParser::EFileType::MODEL:    return "Model";
	case AmberEngine::Tools::Utils::PathParser::EFileType::TEXTURE:  return "Texture";
	case AmberEngine::Tools::Utils::PathParser::EFileType::SHADER:   return "Shader";
	case AmberEngine::Tools::Utils::PathParser::EFileType::MATERIAL:   return "Material";
	}

	return "Unknown";
}

AmberEngine::Tools::Utils::PathParser::EFileType AmberEngine::Tools::Utils::PathParser::GetFileType(const std::string& p_path)
{
	std::string ext = GetExtension(p_path);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	if (ext == "fbx" || ext == "obj")
		return EFileType::MODEL;
	else if (ext == "png" || ext == "jpeg" || ext == "jpg" || ext == "tga")
		return EFileType::TEXTURE;
	else if (ext == "glsl")
		return EFileType::SHADER;
	else if (ext == "abmat")													return EFileType::MATERIAL;

	return EFileType::UNKNOWN;
}
