#include "Amberpch.h"

#include "AmberDebug/HistoryHandler.h"

std::queue<AmberDebug::LogData> AmberDebug::HistoryHandler::LOG_QUEUE;

void AmberDebug::HistoryHandler::Log(const LogData& p_logData)
{
	LOG_QUEUE.push(p_logData);
}

std::queue<AmberDebug::LogData>& AmberDebug::HistoryHandler::GetLogQueue()
{
	return LOG_QUEUE;
}