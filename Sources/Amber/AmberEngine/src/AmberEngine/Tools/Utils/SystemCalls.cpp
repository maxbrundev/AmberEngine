#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/PathParser.h"
#include "AmberEngine/Tools/Utils/SystemCalls.h"

#include <Windows.h>

void AmberEngine::Tools::Utils::SystemCalls::ShowInExplorer(const std::string& p_path)
{
	ShellExecuteA(NULL, "open", PathParser::MakeWindowsStyle(p_path).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void AmberEngine::Tools::Utils::SystemCalls::OpenFile(const std::string& p_file, const std::string& p_workingDir)
{
	ShellExecuteA(NULL, "open", Utils::PathParser::MakeWindowsStyle(p_file).c_str(), NULL,
		p_workingDir.empty() ? NULL : Utils::PathParser::MakeWindowsStyle(p_workingDir).c_str(),
		SW_SHOWNORMAL);
}

void AmberEngine::Tools::Utils::SystemCalls::EditFile(const std::string& p_file)
{
	ShellExecuteW(NULL, NULL, std::wstring(p_file.begin(), p_file.end()).c_str(), NULL, NULL, SW_NORMAL);
}

void AmberEngine::Tools::Utils::SystemCalls::OpenURL(const std::string& p_url)
{
	ShellExecute(0, 0, p_url.c_str(), 0, 0, SW_SHOW);
}
