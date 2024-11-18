#include "Amberpch.h"

#include "AmberEngine/Debug/HistoryHandler.h"

std::queue<AmberEngine::Debug::LogData> AmberEngine::Debug::HistoryHandler::LOG_QUEUE;

void AmberEngine::Debug::HistoryHandler::Log(const LogData& p_logData)
{
	LOG_QUEUE.push(p_logData);
}

std::queue<AmberEngine::Debug::LogData>& AmberEngine::Debug::HistoryHandler::GetLogQueue()
{
	return LOG_QUEUE;
}