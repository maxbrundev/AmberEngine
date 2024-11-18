#include "Amberpch.h"

#define _CRT_SECURE_NO_WARNINGS // Enable getenv

#include "AmberEngine/Debug/FileHandler.h"

#include <fstream>
#include <iostream>

#include "AmberEngine/Debug/HandlerHeader.h"
#include "AmberEngine/Tools/Time/Date.h"

std::string AmberEngine::Debug::FileHandler::APP_LAUNCH_DATE = Tools::Time::Date::GetDateAsString();
std::string const AmberEngine::Debug::FileHandler::LOG_EXTENSION = ".ablog";

std::string AmberEngine::Debug::FileHandler::LOG_FILE_PATH = std::string(getenv("APPDATA")) + std::string("\\AmberTech\\ABEditor\\Log\\") + APP_LAUNCH_DATE + LOG_EXTENSION;

std::ofstream AmberEngine::Debug::FileHandler::OUTPUT_FILE;

void AmberEngine::Debug::FileHandler::Log(const LogData& p_logData)
{
	if (!OUTPUT_FILE.is_open())
		OUTPUT_FILE.open(LOG_FILE_PATH, std::ios_base::app);

	if (OUTPUT_FILE.is_open())
		OUTPUT_FILE << GetLogHeader(p_logData.logLevel) << p_logData.date << " " << p_logData.message << std::endl;
	else
		std::cout << "Unable to create log file" << std::endl;
}

std::string& AmberEngine::Debug::FileHandler::GetLogFilePath()
{
	return LOG_FILE_PATH;
}

void AmberEngine::Debug::FileHandler::SetLogFilePath(const std::string& p_path)
{
	int index = static_cast<int>(p_path.size() - 1);

	while (p_path[index] != '/')
	{
		index--;
	}

	std::string fileName = p_path.substr(index + 1, p_path.size() - 1);

	LOG_FILE_PATH = p_path.substr(0, index + 1) + APP_LAUNCH_DATE + fileName;
}

std::string AmberEngine::Debug::FileHandler::GetLogHeader(ELogLevel p_logLevel)
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