#pragma once

namespace AmberEngine::Debug
{
	enum class ELogLevel
	{
		LOG_DEFAULT,
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR
	};

	enum class ELogMode
	{
		DEFAULT,
		CONSOLE,
		FILE,
		HISTORY,
		ALL
	};

	struct LogData
	{
		ELogLevel logLevel;
		std::string message;
		std::string date;
	};

	class ILogHandler
	{
		friend class Logger;

	private:
		virtual void Log(const LogData& p_logData) = 0;
	};
}