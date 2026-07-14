#include "Amberpch.h"

#include "AmberTools/FileSystem/IniFile.h"

#include <filesystem>
#include <fstream>

AmberTools::FileSystem::IniFile::IniFile(const std::string& p_filePath) : m_filePath(p_filePath)
{
	Load();
}

void AmberTools::FileSystem::IniFile::Reload()
{
	RemoveAll();
	Load();
}

bool AmberTools::FileSystem::IniFile::Remove(const std::string & p_key)
{
	if (IsKeyExisting(p_key))
	{
		m_data.erase(p_key);
		return true;
	}

	return false;
}

void AmberTools::FileSystem::IniFile::RemoveAll()
{
	m_data.clear();
}

bool AmberTools::FileSystem::IniFile::IsKeyExisting(const std::string& p_key) const
{
	return m_data.find(p_key) != m_data.end();
}

void AmberTools::FileSystem::IniFile::RegisterPair(const std::string& p_key, const std::string& p_value)
{
	RegisterPair(std::make_pair(p_key, p_value));
}

void AmberTools::FileSystem::IniFile::RegisterPair(const AttributePair& p_pair)
{
	m_data.insert(p_pair);
}

std::vector<std::string> AmberTools::FileSystem::IniFile::GetFormattedContent() const
{
	std::vector<std::string> result;

	for (const auto&[key, value] : m_data)
		result.push_back(key + "=" + value);

	return result;
}

void AmberTools::FileSystem::IniFile::Load()
{
	std::fstream iniFile;
	iniFile.open(m_filePath);

	if (iniFile.is_open())
	{
		std::string currentLine;

		while (std::getline(iniFile, currentLine))
		{
			if (IsValidLine(currentLine))
			{
				RegisterPair(ExtractKeyAndValue(currentLine));
			}
		}

		iniFile.close();
	}
}

void AmberTools::FileSystem::IniFile::Rewrite() const
{
	std::ofstream outfile;
	outfile.open(m_filePath, std::ios_base::trunc);

	if (outfile.is_open())
	{
		for (const auto&[key, value] : m_data)
			outfile << key << "=" << value << std::endl;
	}

	outfile.close();
}

static std::string TrimWhitespace(const std::string& p_string)
{
	const size_t first = p_string.find_first_not_of(" \t\r");

	if (first == std::string::npos)
		return "";

	const size_t last = p_string.find_last_not_of(" \t\r");

	return p_string.substr(first, last - first + 1);
}

std::pair<std::string, std::string> AmberTools::FileSystem::IniFile::ExtractKeyAndValue(const std::string& p_line) const
{
	std::string key;
	std::string value;

	std::string* currentBuffer = &key;

	for (auto& c : p_line)
	{
		if (c == '=' && currentBuffer == &key)
			currentBuffer = &value;
		else
			currentBuffer->push_back(c);
	}

	return std::make_pair(TrimWhitespace(key), TrimWhitespace(value));
}

bool AmberTools::FileSystem::IniFile::IsValidLine(const std::string & p_attributeLine) const
{
	if (p_attributeLine.size() == 0)
		return false;

	if (p_attributeLine[0] == '#' || p_attributeLine[0] == ';' || p_attributeLine[0] == '[')
		return false;

	if (std::count(p_attributeLine.begin(), p_attributeLine.end(), '=') != 1)
		return false;

	return true;
}

bool AmberTools::FileSystem::IniFile::StringToBoolean(const std::string & p_value) const
{
	return (p_value == "1" || p_value == "T" || p_value == "t" || p_value == "True" || p_value == "true");
}
