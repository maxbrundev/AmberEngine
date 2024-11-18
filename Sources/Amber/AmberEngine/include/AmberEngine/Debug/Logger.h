#pragma once

#include <map>
#include <string>

#include "AmberEngine/Debug/ConsoleHandler.h"
#include "AmberEngine/Debug/FileHandler.h"
#include "AmberEngine/Debug/HistoryHandler.h"
#include "AmberEngine/Debug/ILogHandler.h"

#include "AmberEngine/Eventing/Event.h"

#define AB_LOG(message)         AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_DEFAULT, AmberEngine::Debug::ELogMode::CONSOLE)
#define AB_LOG_INFO(message)    AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_INFO,    AmberEngine::Debug::ELogMode::CONSOLE)
#define AB_LOG_WARNING(message) AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_WARNING, AmberEngine::Debug::ELogMode::CONSOLE)
#define AB_LOG_ERROR(message)   AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_ERROR,   AmberEngine::Debug::ELogMode::CONSOLE)

#define AB_FLOG(message)         AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_DEFAULT, AmberEngine::Debug::ELogMode::CONSOLE); AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_DEFAULT, AmberEngine::Debug::ELogMode::FILE)
#define AB_FLOG_INFO(message)    AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_INFO,    AmberEngine::Debug::ELogMode::CONSOLE); AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_INFO,    AmberEngine::Debug::ELogMode::FILE)
#define AB_FLOG_WARNING(message) AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_WARNING, AmberEngine::Debug::ELogMode::CONSOLE); AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_WARNING, AmberEngine::Debug::ELogMode::FILE)
#define AB_FLOG_ERROR(message)   AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_ERROR,   AmberEngine::Debug::ELogMode::CONSOLE); AmberEngine::Debug::Logger::Log(message, AmberEngine::Debug::ELogLevel::LOG_ERROR,   AmberEngine::Debug::ELogMode::FILE)

namespace AmberEngine::Debug
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
		static Eventing::Event<const LogData&> LogEvent;

	private:
		static std::map<std::string, ConsoleHandler> CONSOLE_HANDLER_MAP;
		static std::map<std::string, FileHandler>    FILE_HANDLER_MAP;
		static std::map<std::string, HistoryHandler> HISTORY_HANDLER_MAP;
	};
}

#include "AmberEngine/Debug/Logger.inl"