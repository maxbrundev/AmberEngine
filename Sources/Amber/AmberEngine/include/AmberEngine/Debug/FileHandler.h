#pragma once

#include <string>

#include "AmberEngine/Debug/ILogHandler.h"

namespace AmberEngine::Debug
{
	class FileHandler final : public AmberEngine::Debug::ILogHandler
	{
		friend class Logger;

	public:
		static std::string& GetLogFilePath();
		static void SetLogFilePath(const std::string& p_path);

	private:
		void Log(const LogData& p_logData) override;

		static std::string GetLogHeader(ELogLevel p_logLevel);

	private:
		static const std::string LOG_EXTENSION;

		static std::string APP_LAUNCH_DATE;
		static std::string LOG_FILE_PATH;

		static std::ofstream OUTPUT_FILE;
	};
}