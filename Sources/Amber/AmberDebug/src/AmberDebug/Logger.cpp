#include "Amberpch.h"

#include "AmberDebug/Logger.h"
#include "AmberTools/Time/Date.h"

AmberTools::Eventing::Event<const AmberDebug::LogData&> AmberDebug::Logger::LogEvent;

std::map<std::string, AmberDebug::ConsoleHandler> AmberDebug::Logger::CONSOLE_HANDLER_MAP;
std::map<std::string, AmberDebug::FileHandler> AmberDebug::Logger::FILE_HANDLER_MAP;
std::map<std::string, AmberDebug::HistoryHandler> AmberDebug::Logger::HISTORY_HANDLER_MAP;

void AmberDebug::Logger::Log(const std::string& p_message, ELogLevel p_logLevel, ELogMode p_logMode, std::string p_handlerID)
{
	LogData logData
	{
		p_logLevel,
		p_message,
		AmberTools::Time::Date::GetDateAsString()
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

AmberDebug::ConsoleHandler& AmberDebug::Logger::CreateConsoleHandler(std::string p_id)
{
	CONSOLE_HANDLER_MAP.emplace(p_id, AmberDebug::ConsoleHandler());
	return CONSOLE_HANDLER_MAP[p_id];
}

AmberDebug::FileHandler& AmberDebug::Logger::CreateFileHandler(std::string p_id)
{
	FILE_HANDLER_MAP.emplace(p_id, AmberDebug::FileHandler());
	return FILE_HANDLER_MAP[p_id];
}

AmberDebug::HistoryHandler& AmberDebug::Logger::CreateHistoryHandler(std::string p_id)
{
	HISTORY_HANDLER_MAP.emplace(p_id, AmberDebug::HistoryHandler());
	return HISTORY_HANDLER_MAP[p_id];
}

AmberDebug::ConsoleHandler& AmberDebug::Logger::GetConsoleHandler(std::string p_id)
{
	return CONSOLE_HANDLER_MAP[p_id];
}

AmberDebug::FileHandler& AmberDebug::Logger::GetFileHandler(std::string p_id)
{
	return FILE_HANDLER_MAP[p_id];
}

AmberDebug::HistoryHandler& AmberDebug::Logger::GetHistoryHandler(std::string p_id)
{
	return HISTORY_HANDLER_MAP[p_id];
}
