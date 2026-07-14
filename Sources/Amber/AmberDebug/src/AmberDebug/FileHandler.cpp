#include "Amberpch.h"

#define _CRT_SECURE_NO_WARNINGS // Enable getenv

#include "AmberDebug/FileHandler.h"

#include <fstream>
#include <iostream>

#include "AmberDebug/HandlerHeader.h"
#include "AmberTools/Time/Date.h"

std::string AmberDebug::FileHandler::APP_LAUNCH_DATE = AmberTools::Time::Date::GetDateAsString();
std::string const AmberDebug::FileHandler::LOG_EXTENSION = ".ablog";

std::string AmberDebug::FileHandler::LOG_FILE_PATH = std::string(getenv("APPDATA")) + std::string("\\AmberTech\\ABEditor\\Log\\") + APP_LAUNCH_DATE + LOG_EXTENSION;

std::ofstream AmberDebug::FileHandler::OUTPUT_FILE;

void AmberDebug::FileHandler::Log(const LogData& p_logData)
{
	if (!OUTPUT_FILE.is_open())
		OUTPUT_FILE.open(LOG_FILE_PATH, std::ios_base::app);

	if (OUTPUT_FILE.is_open())
		OUTPUT_FILE << GetLogHeader(p_logData.logLevel) << p_logData.date << " " << p_logData.message << std::endl;
	else
		std::cout << "Unable to create log file" << std::endl;
}

std::string& AmberDebug::FileHandler::GetLogFilePath()
{
	return LOG_FILE_PATH;
}

void AmberDebug::FileHandler::SetLogFilePath(const std::string& p_path)
{
	int index = static_cast<int>(p_path.size() - 1);

	while (p_path[index] != '/')
	{
		index--;
	}

	std::string fileName = p_path.substr(index + 1, p_path.size() - 1);

	LOG_FILE_PATH = p_path.substr(0, index + 1) + APP_LAUNCH_DATE + fileName;
}

std::string AmberDebug::FileHandler::GetLogHeader(ELogLevel p_logLevel)
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