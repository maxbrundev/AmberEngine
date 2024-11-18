#include "Amberpch.h"

#include "AmberEngine/Debug/Logger.h"
#include "AmberEngine/Tools/Time/Date.h"

AmberEngine::Eventing::Event<const AmberEngine::Debug::LogData&> AmberEngine::Debug::Logger::LogEvent;

std::map<std::string, AmberEngine::Debug::ConsoleHandler> AmberEngine::Debug::Logger::CONSOLE_HANDLER_MAP;
std::map<std::string, AmberEngine::Debug::FileHandler> AmberEngine::Debug::Logger::FILE_HANDLER_MAP;
std::map<std::string, AmberEngine::Debug::HistoryHandler> AmberEngine::Debug::Logger::HISTORY_HANDLER_MAP;

void AmberEngine::Debug::Logger::Log(const std::string& p_message, ELogLevel p_logLevel, ELogMode p_logMode, std::string p_handlerID)
{
	LogData logData
	{
		p_logLevel,
		p_message,
		Tools::Time::Date::GetDateAsString()
	};

	switch (p_logMode)
	{
	case ELogMode::DEFAULT:
	case ELogMode::CONSOLE: LogToHandlerMap<ConsoleHandler>(CONSOLE_HANDLER_MAP, logData, p_handlerID); break;

	case ELogMode::FILE: LogToHandlerMap<FileHandler>(FILE_HANDLER_MAP, logData, p_handlerID);		break;
	case ELogMode::HISTORY: LogToHandlerMap<HistoryHandler>(HISTORY_HANDLER_MAP, logData, p_handlerID);	break;

	case ELogMode::ALL:
		LogToHandlerMap<ConsoleHandler>(CONSOLE_HANDLER_MAP, logData, p_handlerID);
		LogToHandlerMap<FileHandler>(FILE_HANDLER_MAP, logData, p_handlerID);
		LogToHandlerMap<HistoryHandler>(HISTORY_HANDLER_MAP, logData, p_handlerID);
		break;
	}

	LogEvent.Invoke(logData);
}

AmberEngine::Debug::ConsoleHandler& AmberEngine::Debug::Logger::CreateConsoleHandler(std::string p_id)
{
	CONSOLE_HANDLER_MAP.emplace(p_id, AmberEngine::Debug::ConsoleHandler());
	return CONSOLE_HANDLER_MAP[p_id];
}

AmberEngine::Debug::FileHandler& AmberEngine::Debug::Logger::CreateFileHandler(std::string p_id)
{
	FILE_HANDLER_MAP.emplace(p_id, AmberEngine::Debug::FileHandler());
	return FILE_HANDLER_MAP[p_id];
}

AmberEngine::Debug::HistoryHandler& AmberEngine::Debug::Logger::CreateHistoryHandler(std::string p_id)
{
	HISTORY_HANDLER_MAP.emplace(p_id, AmberEngine::Debug::HistoryHandler());
	return HISTORY_HANDLER_MAP[p_id];
}

AmberEngine::Debug::ConsoleHandler& AmberEngine::Debug::Logger::GetConsoleHandler(std::string p_id)
{
	return CONSOLE_HANDLER_MAP[p_id];
}

AmberEngine::Debug::FileHandler& AmberEngine::Debug::Logger::GetFileHandler(std::string p_id)
{
	return FILE_HANDLER_MAP[p_id];
}

AmberEngine::Debug::HistoryHandler& AmberEngine::Debug::Logger::GetHistoryHandler(std::string p_id)
{
	return HISTORY_HANDLER_MAP[p_id];
}
