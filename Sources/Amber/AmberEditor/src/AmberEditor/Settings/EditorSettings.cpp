#include "Amberpch.h"

#include "AmberEditor/Settings/EditorSettings.h"

#include "AmberTools/FileSystem/IniFile.h"

void AmberEditor::Settings::EditorSettings::Save()
{
	AmberTools::FileSystem::IniFile iniFile(std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\editor.ini");
	iniFile.RemoveAll();
	iniFile.Add("latest_layout", LatestLayout.Get());
	iniFile.Rewrite();
}

void AmberEditor::Settings::EditorSettings::Load()
{
	AmberTools::FileSystem::IniFile iniFile(std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\editor.ini");

	LatestLayout = iniFile.GetOrDefault<std::string>("latest_layout", "");
}
