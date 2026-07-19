#pragma once

namespace AmberTools::Utils
{
	class SystemCalls
	{
	public:
		SystemCalls() = delete;

		static void ShowInExplorer(const std::string& p_path);

		static void OpenFile(const std::string& p_file, const std::string & p_workingDir = "");

		static void EditFile(const std::string& p_file);

		static void OpenURL(const std::string& p_url);
	};
}