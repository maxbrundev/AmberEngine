#pragma once

#include <queue>

#include "AmberEngine/Debug/ILogHandler.h"

namespace AmberEngine::Debug
{
	class HistoryHandler final : public ILogHandler
	{
		friend class Logger;

	public:
		static std::queue<LogData>& GetLogQueue();

	private:
		void Log(const LogData& p_logData) override;

	private:
		static std::queue<LogData> LOG_QUEUE;
	};
}