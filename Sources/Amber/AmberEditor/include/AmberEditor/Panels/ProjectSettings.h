#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberTools/FileSystem/IniFile.h"

namespace AmberEditor::Panels
{
	class ProjectSettings : public AmberUI::Panels::APanelWindow
	{
	public:
		ProjectSettings(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings);

		template <typename T>
		std::function<T()> GenerateGatherer(const std::string& p_keyName)
		{
			return std::bind(&AmberTools::FileSystem::IniFile::Get<T>, &m_projectFile, p_keyName);
		}

		template <typename T>
		std::function<void(T)> GenerateProvider(const std::string& p_keyName)
		{
			return std::bind(&AmberTools::FileSystem::IniFile::Set<T>, &m_projectFile, p_keyName, std::placeholders::_1);
		}

	private:
		AmberTools::FileSystem::IniFile& m_projectFile;
	};
}
