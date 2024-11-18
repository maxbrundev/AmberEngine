#pragma once

#include "APanelWindow.h"
#include "AmberEngine/Tools/FileSystem/IniFile.h"

namespace AmberEngine::UI::Panels
{
	class ProjectSettings : public APanelWindow
	{
	public:
		ProjectSettings(const std::string& p_title, bool p_opened, const PanelSettings& p_windowSettings);

		template <typename T>
		std::function<T()> GenerateGatherer(const std::string& p_keyName)
		{
			return std::bind(&Tools::Filesystem::IniFile::Get<T>, &m_projectFile, p_keyName);
		}

		template <typename T>
		std::function<void(T)> GenerateProvider(const std::string& p_keyName)
		{
			return std::bind(&Tools::Filesystem::IniFile::Set<T>, &m_projectFile, p_keyName, std::placeholders::_1);
		}

	private:
		Tools::Filesystem::IniFile& m_projectFile;
	};
}
