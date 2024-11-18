#include "Amberpch.h"

#include "AmberEngine/Debug/ConsoleHandler.h"

#include <iostream>

#include "AmberEngine/Debug/ConsoleColor.h"
#include "AmberEngine/Debug/HandlerHeader.h"

void AmberEngine::Debug::ConsoleHandler::Log(const LogData& p_logData)
{
	switch (p_logData.logLevel)
	{
	case ELogLevel::LOG_DEFAULT:
		std::cout << COLOR_WHITE;
		break;
	case ELogLevel::LOG_INFO:
		std::cout << COLOR_BLUE;
		break;
	case ELogLevel::LOG_WARNING:
		std::cout << COLOR_YELLOW;
		break;
	case ELogLevel::LOG_ERROR:
		std::cout << COLOR_RED;
		break;
	}

	std::ostream& output = p_logData.logLevel == ELogLevel::LOG_ERROR ? std::cerr : std::cout;

	output << GetLogHeader(p_logData.logLevel) << p_logData.date << " " << p_logData.message << std::endl;

	std::cout << COLOR_DEFAULT;
}

std::string AmberEngine::Debug::ConsoleHandler::GetLogHeader(ELogLevel p_logLevel)
{
	switch (p_logLevel)
	{
	case ELogLevel::LOG_DEFAULT: return HandlerHeader::DEFAULT_HEADER;
	case ELogLevel::LOG_INFO:    return HandlerHeader::INFO_HEADER;
	case ELogLevel::LOG_WARNING: return HandlerHeader::WARNING_HEADER;
	case ELogLevel::LOG_ERROR:   return HandlerHeader::ERROR_HEADER;
	}

	return "";
}
