#pragma once

#include <map>
#include <string>

#include "AmberDebug/ConsoleHandler.h"
#include "AmberDebug/FileHandler.h"
#include "AmberDebug/HistoryHandler.h"
#include "AmberDebug/ILogHandler.h"

#include "AmberTools/Eventing/Event.h"

#define AB_LOG(message)         AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_DEFAULT, AmberDebug::ELogMode::CONSOLE)
#define AB_LOG_INFO(message)    AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_INFO,    AmberDebug::ELogMode::CONSOLE)
#define AB_LOG_WARNING(message) AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_WARNING, AmberDebug::ELogMode::CONSOLE)
#define AB_LOG_ERROR(message)   AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_ERROR,   AmberDebug::ELogMode::CONSOLE)

#define AB_FLOG(message)         AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_DEFAULT, AmberDebug::ELogMode::CONSOLE); AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_DEFAULT, AmberDebug::ELogMode::FILE)
#define AB_FLOG_INFO(message)    AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_INFO,    AmberDebug::ELogMode::CONSOLE); AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_INFO,    AmberDebug::ELogMode::FILE)
#define AB_FLOG_WARNING(message) AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_WARNING, AmberDebug::ELogMode::CONSOLE); AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_WARNING, AmberDebug::ELogMode::FILE)
#define AB_FLOG_ERROR(message)   AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_ERROR,   AmberDebug::ELogMode::CONSOLE); AmberDebug::Logger::Log(message, AmberDebug::ELogLevel::LOG_ERROR,   AmberDebug::ELogMode::FILE)

namespace AmberDebug
{
	class Logger
	{
	public:
		Logger() = delete;

		static void Log(const std::string& p_message, ELogLevel p_logLevel = ELogLevel::LOG_DEFAULT, ELogMode p_logMode = ELogMode::DEFAULT, std::string p_handlerID = "default");

		static ConsoleHandler& CreateConsoleHandler(std::string p_id);

		static FileHandler& CreateFileHandler(std::string p_id);

		static HistoryHandler& CreateHistoryHandler(std::string p_id);

		static ConsoleHandler& GetConsoleHandler(std::string p_id);

		static FileHandler& GetFileHandler(std::string p_id);

		static HistoryHandler& GetHistoryHandler(std::string p_id);

	private:
		template<typename T>
		static void LogToHandlerMap(std::map<std::string, T>& p_map, const LogData& p_data, std::string p_id);

	public:
		static AmberTools::Eventing::Event<const LogData&> LogEvent;

	private:
		static std::map<std::string, ConsoleHandler> CONSOLE_HANDLER_MAP;
		static std::map<std::string, FileHandler>    FILE_HANDLER_MAP;
		static std::map<std::string, HistoryHandler> HISTORY_HANDLER_MAP;
	};
}

#include "AmberDebug/Logger.inl"